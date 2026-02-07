#!/usr/bin/env python3
"""
Generate realistic Bareos test data with different company sizes.

Supports three company sizes:
- small:  Small company (~6 servers, basic infrastructure)
- medium: Mid-sized company (~12 servers, standard departments)
- large:  Enterprise (~50-200 servers, multiple locations, cloud services)

Generates:
- Clients
- Jobs (Full, Incremental, Differential)
- Job Logs
- Pools
- Filesets

Usage:
    python generate_bareos_testdata.py --size medium [options]

Environment Variables:
    DB_TYPE, DB_HOST, DB_PORT, DB_NAME, DB_USER, DB_PASSWORD
    PGHOST, PGPORT, PGDATABASE, PGUSER, PGPASSWORD

Requirements:
    pip install psycopg2-binary  # for PostgreSQL
    pip install mysql-connector-python  # for MySQL
"""

import argparse
import os
import random
import sys
from datetime import datetime, timedelta
from typing import List, Dict, Tuple, Optional
from dataclasses import dataclass
from enum import Enum

# Try to import database connectors
try:
    import psycopg2
    HAS_PSYCOPG2 = True
except ImportError:
    HAS_PSYCOPG2 = False

try:
    import mysql.connector
    HAS_MYSQL = True
except ImportError:
    HAS_MYSQL = False


# =============================================================================
# Server Types and Profiles
# =============================================================================

class ServerType(Enum):
    """Types of servers."""
    WINDOWS_DC = "Windows Domain Controller"
    WINDOWS_FILE = "Windows File Server"
    WINDOWS_APP = "Windows Application Server"
    LINUX_WEB = "Linux Web Server"
    LINUX_APP = "Linux Application Server"
    LINUX_DB = "Linux Database Server"
    WINDOWS_DB = "Windows SQL Server"
    MAIL_EXCHANGE = "Exchange Mail Server"
    MAIL_LINUX = "Linux Mail Server"
    NAS_SYNOLOGY = "Synology NAS"
    NAS_QNAP = "QNAP NAS"
    NAS_NETAPP = "NetApp Storage"
    VMWARE_HOST = "VMware ESXi Host"
    HYPERV_HOST = "Hyper-V Host"
    CLOUD_AWS = "AWS Cloud Backup"
    CLOUD_AZURE = "Azure Cloud Backup"
    CLOUD_GCP = "Google Cloud Backup"
    SAP_HANA = "SAP HANA Server"
    ORACLE_DB = "Oracle Database Server"
    MSSQL_DB = "MS SQL Server"
    POSTGRES_DB = "PostgreSQL Server"
    MONGODB = "MongoDB Server"
    KUBERNETES = "Kubernetes Cluster"
    DOCKER_HOST = "Docker Host"


@dataclass
class ServerProfile:
    """Backup characteristics for a server type."""
    server_type: ServerType
    files_min: int
    files_max: int
    size_gb_min: float
    size_gb_max: float
    incr_ratio: float
    failure_rate: float
    warning_rate: float
    duration_hours_min: float
    duration_hours_max: float


# Server profiles with realistic backup characteristics
SERVER_PROFILES: Dict[ServerType, ServerProfile] = {
    ServerType.WINDOWS_DC: ServerProfile(
        ServerType.WINDOWS_DC, 30000, 100000, 15, 50, 0.05, 0.02, 0.04, 0.3, 1.5
    ),
    ServerType.WINDOWS_FILE: ServerProfile(
        ServerType.WINDOWS_FILE, 200000, 2000000, 200, 2000, 0.02, 0.03, 0.06, 1, 8
    ),
    ServerType.WINDOWS_APP: ServerProfile(
        ServerType.WINDOWS_APP, 50000, 200000, 30, 150, 0.04, 0.03, 0.05, 0.5, 2
    ),
    ServerType.LINUX_WEB: ServerProfile(
        ServerType.LINUX_WEB, 5000, 50000, 2, 30, 0.1, 0.02, 0.03, 0.2, 0.8
    ),
    ServerType.LINUX_APP: ServerProfile(
        ServerType.LINUX_APP, 30000, 200000, 15, 100, 0.05, 0.03, 0.05, 0.4, 2
    ),
    ServerType.LINUX_DB: ServerProfile(
        ServerType.LINUX_DB, 500, 20000, 50, 1000, 0.1, 0.02, 0.04, 0.5, 4
    ),
    ServerType.WINDOWS_DB: ServerProfile(
        ServerType.WINDOWS_DB, 500, 20000, 50, 1000, 0.1, 0.03, 0.05, 0.5, 4
    ),
    ServerType.MAIL_EXCHANGE: ServerProfile(
        ServerType.MAIL_EXCHANGE, 100000, 1000000, 200, 2000, 0.03, 0.04, 0.08, 2, 8
    ),
    ServerType.MAIL_LINUX: ServerProfile(
        ServerType.MAIL_LINUX, 50000, 500000, 100, 1000, 0.04, 0.03, 0.06, 1, 5
    ),
    ServerType.NAS_SYNOLOGY: ServerProfile(
        ServerType.NAS_SYNOLOGY, 500000, 5000000, 500, 10000, 0.02, 0.02, 0.04, 2, 16
    ),
    ServerType.NAS_QNAP: ServerProfile(
        ServerType.NAS_QNAP, 500000, 5000000, 500, 10000, 0.02, 0.02, 0.04, 2, 16
    ),
    ServerType.NAS_NETAPP: ServerProfile(
        ServerType.NAS_NETAPP, 2000000, 20000000, 2000, 50000, 0.01, 0.01, 0.03, 4, 24
    ),
    ServerType.VMWARE_HOST: ServerProfile(
        ServerType.VMWARE_HOST, 50, 500, 200, 2000, 0.03, 0.03, 0.06, 1, 8
    ),
    ServerType.HYPERV_HOST: ServerProfile(
        ServerType.HYPERV_HOST, 50, 500, 200, 2000, 0.03, 0.03, 0.06, 1, 8
    ),
    ServerType.CLOUD_AWS: ServerProfile(
        ServerType.CLOUD_AWS, 10000, 200000, 30, 500, 0.05, 0.05, 0.1, 0.5, 4
    ),
    ServerType.CLOUD_AZURE: ServerProfile(
        ServerType.CLOUD_AZURE, 10000, 200000, 30, 500, 0.05, 0.05, 0.1, 0.5, 4
    ),
    ServerType.CLOUD_GCP: ServerProfile(
        ServerType.CLOUD_GCP, 10000, 200000, 30, 500, 0.05, 0.05, 0.1, 0.5, 4
    ),
    ServerType.SAP_HANA: ServerProfile(
        ServerType.SAP_HANA, 5000, 50000, 500, 5000, 0.05, 0.02, 0.04, 2, 8
    ),
    ServerType.ORACLE_DB: ServerProfile(
        ServerType.ORACLE_DB, 1000, 30000, 200, 3000, 0.08, 0.02, 0.04, 1, 6
    ),
    ServerType.MSSQL_DB: ServerProfile(
        ServerType.MSSQL_DB, 1000, 30000, 100, 2000, 0.1, 0.03, 0.05, 1, 5
    ),
    ServerType.POSTGRES_DB: ServerProfile(
        ServerType.POSTGRES_DB, 1000, 20000, 50, 800, 0.1, 0.02, 0.03, 0.5, 3
    ),
    ServerType.MONGODB: ServerProfile(
        ServerType.MONGODB, 1000, 50000, 100, 1500, 0.08, 0.03, 0.05, 0.5, 4
    ),
    ServerType.KUBERNETES: ServerProfile(
        ServerType.KUBERNETES, 20000, 300000, 30, 300, 0.05, 0.04, 0.08, 0.3, 2
    ),
    ServerType.DOCKER_HOST: ServerProfile(
        ServerType.DOCKER_HOST, 10000, 100000, 15, 150, 0.05, 0.03, 0.06, 0.3, 1.5
    ),
}


# =============================================================================
# Company Size Configurations
# =============================================================================

@dataclass
class Location:
    """Company location/site."""
    name: str
    code: str


@dataclass
class ServerConfig:
    """Server configuration for a company."""
    name: str
    server_type: ServerType
    description: str


# Small company infrastructure (~6 servers)
SMALL_COMPANY = {
    'name': "Kleine Firma GmbH",
    'locations': [Location("Hauptsitz", "HQ")],
    'servers': [
        ServerConfig("dc01", ServerType.WINDOWS_DC, "Primary Domain Controller"),
        ServerConfig("dc02", ServerType.WINDOWS_DC, "Secondary Domain Controller"),
        ServerConfig("fs01", ServerType.WINDOWS_FILE, "File Server"),
        ServerConfig("web01", ServerType.LINUX_WEB, "Webserver"),
        ServerConfig("db01", ServerType.LINUX_DB, "Datenbank Server"),
        ServerConfig("nas01", ServerType.NAS_SYNOLOGY, "NAS Backup"),
    ],
    'days_default': 30,
}

# Medium company infrastructure (~12 servers)
MEDIUM_COMPANY = {
    'name': "Mittelstand AG",
    'locations': [Location("Hauptsitz", "HQ")],
    'servers': [
        ServerConfig("dc01", ServerType.WINDOWS_DC, "Domain Controller"),
        ServerConfig("dc02", ServerType.WINDOWS_DC, "Secondary DC"),
        ServerConfig("fs01", ServerType.WINDOWS_FILE, "File Server"),
        ServerConfig("nas01", ServerType.NAS_SYNOLOGY, "Synology NAS"),
        ServerConfig("app01", ServerType.WINDOWS_APP, "ERP System"),
        ServerConfig("app02", ServerType.WINDOWS_APP, "CRM Application"),
        ServerConfig("web01", ServerType.LINUX_WEB, "Company Website"),
        ServerConfig("web02", ServerType.LINUX_WEB, "Intranet Portal"),
        ServerConfig("db01", ServerType.LINUX_DB, "PostgreSQL Database"),
        ServerConfig("sql01", ServerType.WINDOWS_DB, "MS SQL Server"),
        ServerConfig("esx01", ServerType.VMWARE_HOST, "VMware Host 1"),
        ServerConfig("esx02", ServerType.VMWARE_HOST, "VMware Host 2"),
    ],
    'days_default': 60,
}

# Large enterprise infrastructure (~50-200 servers)
LARGE_COMPANY = {
    'name': "Enterprise Konzern GmbH",
    'locations': [
        Location("Frankfurt Datacenter", "FRA"),
        Location("München Office", "MUC"),
        Location("Berlin Office", "BER"),
        Location("London Office", "LON"),
        Location("AWS eu-central-1", "AWS-EU"),
        Location("Azure West Europe", "AZ-WE"),
    ],
    'server_templates': [
        # Core Infrastructure (per location)
        (ServerType.WINDOWS_DC, "dc", 2),
        (ServerType.WINDOWS_FILE, "fs", 2),
        (ServerType.NAS_SYNOLOGY, "nas", 1),
        # Applications
        (ServerType.WINDOWS_APP, "app", 3),
        (ServerType.LINUX_APP, "lapp", 2),
        # Web
        (ServerType.LINUX_WEB, "web", 4),
        # Databases
        (ServerType.LINUX_DB, "pgsql", 2),
        (ServerType.WINDOWS_DB, "mssql", 1),
        (ServerType.ORACLE_DB, "ora", 1),
        (ServerType.MONGODB, "mongo", 1),
        # Virtualization
        (ServerType.VMWARE_HOST, "esx", 3),
        # Containers
        (ServerType.KUBERNETES, "k8s", 1),
        (ServerType.DOCKER_HOST, "docker", 2),
    ],
    'cloud_servers': [
        ServerConfig("aws-backup-01", ServerType.CLOUD_AWS, "AWS S3 Backup"),
        ServerConfig("azure-backup-01", ServerType.CLOUD_AZURE, "Azure Blob Backup"),
        ServerConfig("gcp-backup-01", ServerType.CLOUD_GCP, "Google Cloud Backup"),
    ],
    'days_default': 90,
}


# =============================================================================
# Job Log Templates
# =============================================================================

LOG_TEMPLATES = {
    'start': [
        "JobId {jobid}: Start Backup JobId {jobid}, Job={jobname}",
        "JobId {jobid}: Using Device \"{storage}\" to write.",
        "JobId {jobid}: Volume \"{volume}\" previously written, moving to end of data.",
    ],
    'running': [
        "JobId {jobid}: Sending Accurate information to client {client}.",
        "JobId {jobid}: Connected to client {client} at {client_ip}:9102.",
        "JobId {jobid}: Begin reading from \"{path}\".",
        "JobId {jobid}: Files examined: {files_examined}",
        "JobId {jobid}: Files backed up: {files_backed}",
    ],
    'end_ok': [
        "JobId {jobid}: Backup OK. Files={files} Bytes={bytes} ({bytes_human})",
        "JobId {jobid}: Terminating at {endtime}",
        "JobId {jobid}: Job write elapsed time: {duration}",
        "JobId {jobid}: Backup completed successfully.",
    ],
    'end_warning': [
        "JobId {jobid}: Warning: Some files could not be backed up.",
        "JobId {jobid}: {errors} file(s) had errors during backup.",
        "JobId {jobid}: Backup OK -- with warnings. Files={files} Bytes={bytes}",
        "JobId {jobid}: Terminating at {endtime}",
    ],
    'end_failed': [
        "JobId {jobid}: Error: Backup failed.",
        "JobId {jobid}: Fatal error: {error_msg}",
        "JobId {jobid}: Backup FAILED. Files={files} Bytes={bytes}",
        "JobId {jobid}: Terminating at {endtime}",
    ],
    'errors': [
        "Cannot open file \"{path}\": Permission denied",
        "File \"{path}\" changed during backup.",
        "Cannot stat \"{path}\": No such file or directory",
        "Read error on file \"{path}\": Input/output error",
        "Connection to client lost during backup.",
        "Storage daemon not responding.",
        "Volume \"{volume}\" is full.",
    ],
}

ERROR_MESSAGES = [
    "Connection to storage daemon failed",
    "Cannot contact client daemon",
    "Storage device not available",
    "Network timeout during transfer",
    "Disk quota exceeded on storage",
    "Authentication failed with client",
    "Volume pool exhausted",
]

BACKUP_PATHS = [
    "/home", "/var/www", "/opt/application", "/etc", "/var/lib/mysql",
    "/var/lib/postgresql", "/srv/samba", "/data", "/backup",
    "C:\\Users", "C:\\Program Files", "C:\\Windows\\System32\\config",
    "D:\\Data", "E:\\Shares", "F:\\Databases",
]


# =============================================================================
# Realistic File Path Templates per Server Type
# =============================================================================

FILE_TEMPLATES: Dict[ServerType, Dict] = {
    ServerType.WINDOWS_DC: {
        'paths': [
            "C:/Windows/NTDS",
            "C:/Windows/SYSVOL/domain/Policies",
            "C:/Windows/System32/config",
            "C:/Windows/System32/dns",
            "C:/Users/Administrator/Documents",
        ],
        'files': [
            ("ntds.dit", 500*1024*1024, 2000*1024*1024),
            ("edb.log", 10*1024*1024, 100*1024*1024),
            ("edb.chk", 1024, 8192),
            ("SYSTEM", 10*1024*1024, 50*1024*1024),
            ("SOFTWARE", 50*1024*1024, 200*1024*1024),
            ("SAM", 256*1024, 2*1024*1024),
            ("SECURITY", 256*1024, 1024*1024),
            ("GPT.INI", 256, 1024),
            ("{31B2F340-016D-11D2-945F-00C04FB984F9}/Machine/Registry.pol", 1024, 50*1024),
        ],
    },
    ServerType.WINDOWS_FILE: {
        'paths': [
            "D:/Shares/Abteilung/Buchhaltung",
            "D:/Shares/Abteilung/Personal",
            "D:/Shares/Abteilung/Vertrieb",
            "D:/Shares/Abteilung/IT",
            "D:/Shares/Projekte/2024",
            "D:/Shares/Projekte/2025",
            "D:/Shares/Archiv",
            "D:/Shares/Vorlagen",
            "E:/Backup/Daily",
        ],
        'files': [
            ("Jahresabschluss_2024.xlsx", 500*1024, 5*1024*1024),
            ("Quartalsbericht_Q4.docx", 100*1024, 2*1024*1024),
            ("Kundenliste.xlsx", 1*1024*1024, 20*1024*1024),
            ("Praesentation_Vorstand.pptx", 5*1024*1024, 50*1024*1024),
            ("Vertrag_Kunde_{num}.pdf", 100*1024, 2*1024*1024),
            ("Rechnung_{num}.pdf", 50*1024, 500*1024),
            ("Protokoll_{num}.docx", 50*1024, 500*1024),
            ("Budget_2025.xlsx", 200*1024, 2*1024*1024),
            ("Backup_{num}.zip", 10*1024*1024, 100*1024*1024),
        ],
    },
    ServerType.LINUX_WEB: {
        'paths': [
            "/var/www/html",
            "/var/www/html/css",
            "/var/www/html/js",
            "/var/www/html/images",
            "/var/www/html/uploads",
            "/etc/nginx/sites-available",
            "/etc/nginx/ssl",
            "/var/log/nginx",
        ],
        'files': [
            ("index.html", 1024, 50*1024),
            ("style.css", 5*1024, 100*1024),
            ("app.js", 10*1024, 500*1024),
            ("logo.png", 10*1024, 500*1024),
            ("background.jpg", 100*1024, 2*1024*1024),
            ("favicon.ico", 1024, 10*1024),
            ("robots.txt", 100, 1024),
            (".htaccess", 100, 2048),
            ("access.log", 1*1024*1024, 100*1024*1024),
            ("error.log", 100*1024, 10*1024*1024),
            ("nginx.conf", 2*1024, 10*1024),
            ("ssl.crt", 1*1024, 4*1024),
            ("ssl.key", 1*1024, 4*1024),
            ("upload_{num}.jpg", 50*1024, 5*1024*1024),
        ],
    },
    ServerType.LINUX_APP: {
        'paths': [
            "/opt/application/bin",
            "/opt/application/lib",
            "/opt/application/config",
            "/opt/application/logs",
            "/opt/application/data",
            "/var/log/application",
            "/etc/application",
        ],
        'files': [
            ("application.jar", 50*1024*1024, 200*1024*1024),
            ("config.yml", 1*1024, 50*1024),
            ("application.properties", 1*1024, 20*1024),
            ("log4j2.xml", 2*1024, 10*1024),
            ("application.log", 10*1024*1024, 500*1024*1024),
            ("debug.log", 5*1024*1024, 100*1024*1024),
            ("startup.sh", 1*1024, 10*1024),
            ("shutdown.sh", 512, 5*1024),
            ("libapp.so", 1*1024*1024, 50*1024*1024),
            ("data_{num}.json", 10*1024, 1*1024*1024),
        ],
    },
    ServerType.LINUX_DB: {
        'paths': [
            "/var/lib/postgresql/14/main",
            "/var/lib/postgresql/14/main/base/16384",
            "/var/lib/postgresql/14/main/pg_wal",
            "/etc/postgresql/14/main",
            "/var/log/postgresql",
            "/var/backups/postgresql",
        ],
        'files': [
            ("16385", 100*1024*1024, 1000*1024*1024),
            ("16386", 50*1024*1024, 500*1024*1024),
            ("16387", 20*1024*1024, 200*1024*1024),
            ("000000010000000000000001", 16*1024*1024, 16*1024*1024),
            ("000000010000000000000002", 16*1024*1024, 16*1024*1024),
            ("postgresql.conf", 20*1024, 50*1024),
            ("pg_hba.conf", 1*1024, 10*1024),
            ("postmaster.pid", 100, 200),
            ("postgresql-14-main.log", 1*1024*1024, 100*1024*1024),
            ("pg_dump_full_{num}.sql", 100*1024*1024, 500*1024*1024),
        ],
    },
    ServerType.WINDOWS_DB: {
        'paths': [
            "C:/Program Files/Microsoft SQL Server/MSSQL15.MSSQLSERVER/MSSQL/DATA",
            "C:/Program Files/Microsoft SQL Server/MSSQL15.MSSQLSERVER/MSSQL/Log",
            "C:/Program Files/Microsoft SQL Server/MSSQL15.MSSQLSERVER/MSSQL/Backup",
            "D:/SQLData",
            "E:/SQLBackup",
        ],
        'files': [
            ("master.mdf", 50*1024*1024, 200*1024*1024),
            ("mastlog.ldf", 10*1024*1024, 100*1024*1024),
            ("tempdb.mdf", 100*1024*1024, 1000*1024*1024),
            ("templog.ldf", 50*1024*1024, 500*1024*1024),
            ("model.mdf", 10*1024*1024, 50*1024*1024),
            ("msdbdata.mdf", 20*1024*1024, 100*1024*1024),
            ("ERRORLOG", 1*1024*1024, 50*1024*1024),
            ("AppDB.mdf", 500*1024*1024, 2000*1024*1024),
            ("AppDB.ldf", 100*1024*1024, 500*1024*1024),
            ("AppDB_backup_{num}.bak", 500*1024*1024, 2000*1024*1024),
        ],
    },
    ServerType.NAS_SYNOLOGY: {
        'paths': [
            "/volume1/homes/admin",
            "/volume1/homes/user1",
            "/volume1/photo/2024",
            "/volume1/photo/2025",
            "/volume1/video",
            "/volume1/music",
            "/volume1/backup",
            "/volume1/docker",
            "/volume2/archive",
        ],
        'files': [
            ("DSC_{num}.JPG", 2*1024*1024, 10*1024*1024),
            ("IMG_{num}.RAW", 20*1024*1024, 50*1024*1024),
            ("VID_{num}.MP4", 100*1024*1024, 500*1024*1024),
            ("Track_{num}.mp3", 3*1024*1024, 15*1024*1024),
            ("backup_{num}.tar.gz", 100*1024*1024, 500*1024*1024),
            ("container_{num}.tar", 50*1024*1024, 200*1024*1024),
            ("archive_{num}.zip", 200*1024*1024, 1000*1024*1024),
            ("document_{num}.pdf", 100*1024, 5*1024*1024),
        ],
    },
    ServerType.MAIL_EXCHANGE: {
        'paths': [
            "C:/Program Files/Microsoft/Exchange Server/V15/Mailbox/Mailbox Database",
            "C:/Program Files/Microsoft/Exchange Server/V15/Logging/MessageTracking",
            "C:/Program Files/Microsoft/Exchange Server/V15/TransportRoles/data/Queue",
            "D:/ExchangeDB",
            "E:/ExchangeLogs",
        ],
        'files': [
            ("Mailbox Database.edb", 5000*1024*1024, 50000*1024*1024),
            ("E00.log", 1*1024*1024, 1*1024*1024),
            ("E0000001.log", 1*1024*1024, 1*1024*1024),
            ("E0000002.log", 1*1024*1024, 1*1024*1024),
            ("queue.edb", 100*1024*1024, 500*1024*1024),
            ("trn.log", 1*1024*1024, 10*1024*1024),
            ("MSGTRK{num}.LOG", 10*1024*1024, 50*1024*1024),
        ],
    },
    ServerType.VMWARE_HOST: {
        'paths': [
            "/vmfs/volumes/datastore1",
            "/vmfs/volumes/datastore1/VM-DC01",
            "/vmfs/volumes/datastore1/VM-AppServer",
            "/vmfs/volumes/datastore1/VM-WebServer",
            "/vmfs/volumes/datastore2",
        ],
        'files': [
            ("VM-DC01.vmdk", 5000*1024*1024, 10000*1024*1024),
            ("VM-DC01-flat.vmdk", 5000*1024*1024, 10000*1024*1024),
            ("VM-DC01.vmx", 1*1024, 10*1024),
            ("VM-DC01.nvram", 8*1024, 8*1024),
            ("vmware.log", 1*1024*1024, 50*1024*1024),
            ("VM-AppServer.vmdk", 10000*1024*1024, 50000*1024*1024),
            ("VM-WebServer.vmdk", 2000*1024*1024, 5000*1024*1024),
            ("snapshot-{num}.vmsn", 100*1024*1024, 500*1024*1024),
        ],
    },
    ServerType.KUBERNETES: {
        'paths': [
            "/var/lib/etcd/member/snap",
            "/var/lib/etcd/member/wal",
            "/etc/kubernetes",
            "/etc/kubernetes/pki",
            "/var/log/pods/kube-system",
            "/var/lib/containerd/io.containerd.content.v1.content/blobs/sha256",
        ],
        'files': [
            ("db", 100*1024*1024, 500*1024*1024),
            ("0000000000000001-0000000000000001.wal", 64*1024*1024, 64*1024*1024),
            ("admin.conf", 5*1024, 10*1024),
            ("kubelet.conf", 5*1024, 10*1024),
            ("ca.crt", 1*1024, 2*1024),
            ("ca.key", 1*1024, 2*1024),
            ("apiserver.crt", 1*1024, 2*1024),
            ("kube-apiserver-{num}.log", 10*1024*1024, 100*1024*1024),
            ("{sha256}", 50*1024*1024, 500*1024*1024),
        ],
    },
    ServerType.DOCKER_HOST: {
        'paths': [
            "/var/lib/docker/containers",
            "/var/lib/docker/volumes",
            "/var/lib/docker/image/overlay2/layerdb",
            "/etc/docker",
            "/var/log/docker",
        ],
        'files': [
            ("config.v2.json", 5*1024, 50*1024),
            ("hostconfig.json", 2*1024, 20*1024),
            ("{container_id}-json.log", 1*1024*1024, 100*1024*1024),
            ("_data/app.db", 10*1024*1024, 500*1024*1024),
            ("daemon.json", 1*1024, 10*1024),
            ("sha256/{layer_id}", 10*1024*1024, 200*1024*1024),
        ],
    },
    ServerType.WINDOWS_APP: {
        'paths': [
            "C:/Program Files/Application",
            "C:/Program Files/Application/bin",
            "C:/Program Files/Application/config",
            "C:/Program Files/Application/logs",
            "C:/ProgramData/Application",
            "D:/AppData",
        ],
        'files': [
            ("application.exe", 10*1024*1024, 100*1024*1024),
            ("app.dll", 1*1024*1024, 20*1024*1024),
            ("config.xml", 5*1024, 50*1024),
            ("settings.json", 2*1024, 20*1024),
            ("application.log", 10*1024*1024, 200*1024*1024),
            ("error.log", 1*1024*1024, 50*1024*1024),
            ("data_{num}.dat", 50*1024*1024, 500*1024*1024),
            ("cache_{num}.tmp", 10*1024*1024, 100*1024*1024),
        ],
    },
}

# Default file template for server types not explicitly defined
DEFAULT_FILE_TEMPLATE = {
    'paths': [
        "/data",
        "/var/log",
        "/etc",
        "/opt/app",
    ],
    'files': [
        ("data_{num}.bin", 1*1024*1024, 100*1024*1024),
        ("config.conf", 1*1024, 50*1024),
        ("app.log", 1*1024*1024, 100*1024*1024),
        ("backup_{num}.tar", 50*1024*1024, 500*1024*1024),
    ],
}


# =============================================================================
# Configuration File Templates
# =============================================================================

CONFIG_TEMPLATES = {
    'client': '''Client {{
  Name = "{name}-fd"
  Address = "{address}"
  FD Port = 9102
  Password = "{password}"
  Catalog = MyCatalog
  File Retention = {file_retention} days
  Job Retention = {job_retention} days
  AutoPrune = yes
  # {description}
}}
''',

    'job': '''Job {{
  Name = "Backup-{client_name}"
  Type = Backup
  Level = {level}
  Client = {client_name}-fd
  FileSet = "{fileset}"
  Schedule = "{schedule}"
  Storage = FileStorage
  Pool = {pool}
  Messages = Standard
  Priority = 10
  # {description}
}}
''',

    'fileset_linux': '''FileSet {{
  Name = "{name}"
  Include {{
    Options {{
      Signature = SHA1
      Compression = LZO
    }}
    File = /etc
    File = /home
    File = /var/log
    File = /opt
    File = /srv
  }}
  Exclude {{
    File = /var/cache
    File = /tmp
    File = /proc
    File = /sys
  }}
}}
''',

    'fileset_windows': '''FileSet {{
  Name = "{name}"
  Enable VSS = yes
  Include {{
    Options {{
      Signature = SHA1
      Compression = LZO
      IgnoreCase = yes
    }}
    File = "C:/Users"
    File = "C:/Program Files"
    File = "C:/ProgramData"
  }}
  Exclude {{
    File = "C:/Windows/Temp"
    File = "C:/Users/*/AppData/Local/Temp"
  }}
}}
''',

    'fileset_database': '''FileSet {{
  Name = "{name}"
  Include {{
    Options {{
      Signature = SHA1
      Compression = LZO
    }}
    Plugin = "python:module_name=bareos-fd-postgresql"
  }}
}}
''',

    'pool': '''Pool {{
  Name = "{name}"
  Pool Type = Backup
  Recycle = yes
  AutoPrune = yes
  Volume Retention = {retention} days
  Maximum Volume Bytes = {max_bytes}
  Maximum Volumes = {max_vols}
  Label Format = "Vol-{label_prefix}-"
}}
''',

    'schedule_full_weekly': '''Schedule {{
  Name = "{name}"
  # Full backup on Sunday at 02:00
  Run = Level=Full Pool={full_pool} sun at 02:00
  # Incremental backup every other day at 21:00
  Run = Level=Incremental Pool={incr_pool} mon-sat at 21:00
}}
''',

    'schedule_full_daily': '''Schedule {{
  Name = "{name}"
  # Full backup daily at 01:00
  Run = Level=Full Pool={full_pool} daily at 01:00
  # Incremental backup every 6 hours
  Run = Level=Incremental Pool={incr_pool} hourly at 00:00
  Run = Level=Incremental Pool={incr_pool} hourly at 06:00
  Run = Level=Incremental Pool={incr_pool} hourly at 12:00
  Run = Level=Incremental Pool={incr_pool} hourly at 18:00
}}
''',

    'schedule_always_incremental': '''Schedule {{
  Name = "{name}"
  # Initial Full backup
  Run = Level=Full Pool={full_pool} 1st sun at 01:00
  # Always Incremental after that
  Run = Level=Incremental Pool={incr_pool} daily at 21:00
}}

Job {{
  Name = "Consolidate-{client_base}"
  Type = Consolidate
  Client = {client_base}-fd
  FileSet = "{fileset}"
  Schedule = "{consolidate_schedule}"
  Storage = FileStorage
  Pool = {consolidate_pool}
  Messages = Standard
  Priority = 20
  Max Full Consolidations = 4
  Always Incremental Job Retention = {consolidate_interval} days
  Always Incremental Keep Number = 10
}}

Schedule {{
  Name = "{consolidate_schedule}"
  # Run consolidation weekly on Sunday morning
  Run = sun at 03:00
}}
''',

    'storage': '''Storage {{
  Name = "{name}"
  Address = {address}
  SD Port = 9103
  Password = "{password}"
  Device = FileStorage
  Media Type = File
  Maximum Concurrent Jobs = 10
}}
''',

    'console': '''Console {{
  Name = "{name}"
  Password = "{password}"
  TLS Enable = {tls_enable}
  TLS Require = {tls_require}
  CommandACL = *all*
  ClientAcl = *all*
  JobAcl = *all*
  StorageAcl = *all*
  ScheduleAcl = *all*
  PoolAcl = *all*
  FileSetAcl = *all*
  CatalogAcl = *all*
}}
''',
}


# =============================================================================
# Main Generator Class
# =============================================================================

class BareosTestDataGenerator:
    """Generates Bareos test data for different company sizes."""

    STATUS_OK = 'T'
    STATUS_WARNING = 'W'
    STATUS_FAILED = 'f'
    STATUS_ERROR = 'E'
    STATUS_CANCELED = 'A'

    TYPE_BACKUP = 'B'
    TYPE_CONSOLIDATE = 'c'  # Consolidate job type for Always Incremental
    LEVEL_FULL = 'F'
    LEVEL_INCREMENTAL = 'I'
    LEVEL_DIFFERENTIAL = 'D'
    LEVEL_VIRTUAL_FULL = 'V'  # Virtual Full (result of consolidation)

    def __init__(self, db_type: str, host: str, port: int, database: str,
                 user: str, password: str):
        """Initialize database connection."""
        self.db_type = db_type.lower()
        self.conn = None
        self.cursor = None

        if self.db_type == 'postgresql':
            if not HAS_PSYCOPG2:
                raise ImportError("psycopg2 not installed. Run: pip install psycopg2-binary")
            self.conn = psycopg2.connect(
                host=host, port=port, database=database, user=user, password=password
            )
        elif self.db_type == 'mysql':
            if not HAS_MYSQL:
                raise ImportError("mysql-connector not installed. Run: pip install mysql-connector-python")
            self.conn = mysql.connector.connect(
                host=host, port=port, database=database, user=user, password=password
            )
        else:
            raise ValueError(f"Unsupported database type: {db_type}")

        self.cursor = self.conn.cursor()
        self.script_marker = "generate_bareos_testdata.py"

    def close(self):
        """Close database connection."""
        if self.cursor:
            self.cursor.close()
        if self.conn:
            self.conn.close()

    def get_or_create_pool(self, name: str) -> int:
        """Get or create a pool."""
        self.cursor.execute("SELECT poolid FROM pool WHERE name = %s", (name,))
        result = self.cursor.fetchone()
        if result:
            return result[0]

        self.cursor.execute("""
            INSERT INTO pool (name, numvols, maxvols, useonce, usecatalog,
                             acceptanyvolume, volretention, voluseduration,
                             maxvoljobs, maxvolfiles, maxvolbytes, autoprune,
                             recycle, pooltype, labelformat)
            VALUES (%s, 0, 0, 0, 1, 0, 31536000, 86400, 0, 0, 0, 1, 1, 'Backup', %s)
        """, (name, f"Vol-{name}-"))
        self.conn.commit()

        self.cursor.execute("SELECT poolid FROM pool WHERE name = %s", (name,))
        return self.cursor.fetchone()[0]

    def get_or_create_fileset(self, name: str) -> int:
        """Get or create a fileset."""
        self.cursor.execute("SELECT filesetid FROM fileset WHERE fileset = %s", (name,))
        result = self.cursor.fetchone()
        if result:
            return result[0]

        self.cursor.execute("""
            INSERT INTO fileset (fileset, md5, createtime)
            VALUES (%s, %s, %s)
        """, (name, f"md5-{name}", datetime.now()))
        self.conn.commit()

        self.cursor.execute("SELECT filesetid FROM fileset WHERE fileset = %s", (name,))
        return self.cursor.fetchone()[0]

    def get_or_create_storage(self, name: str) -> int:
        """Get or create a storage."""
        self.cursor.execute("SELECT storageid FROM storage WHERE name = %s", (name,))
        result = self.cursor.fetchone()
        if result:
            return result[0]

        self.cursor.execute("""
            INSERT INTO storage (name, autochanger)
            VALUES (%s, 0)
        """, (name,))
        self.conn.commit()

        self.cursor.execute("SELECT storageid FROM storage WHERE name = %s", (name,))
        return self.cursor.fetchone()[0]

    def create_client(self, name: str, description: str) -> int:
        """Create a client."""
        client_name = f"{name}-fd"
        self.cursor.execute("SELECT clientid FROM client WHERE name = %s", (client_name,))
        result = self.cursor.fetchone()
        if result:
            return result[0]

        self.cursor.execute("""
            INSERT INTO client (name, uname, autoprune, fileretention, jobretention)
            VALUES (%s, %s, 1, 2592000, 15552000)
        """, (client_name, description))
        self.conn.commit()

        self.cursor.execute("SELECT clientid FROM client WHERE name = %s", (client_name,))
        return self.cursor.fetchone()[0]

    def insert_job(self, name: str, client_id: int, pool_id: int, fileset_id: int,
                   job_type: str, level: str, status: str, start_time: datetime,
                   end_time: datetime, job_files: int, job_bytes: int,
                   job_errors: int = 0) -> int:
        """Insert a job record."""
        job_identifier = f"{name}.{start_time.strftime('%Y-%m-%d_%H.%M.%S')}"
        comment = f"Test data - {self.script_marker}"

        if self.db_type == 'postgresql':
            self.cursor.execute("""
                INSERT INTO job (
                    job, name, type, level, clientid, jobstatus,
                    schedtime, starttime, endtime, realendtime,
                    jobtdate, volsessionid, volsessiontime,
                    jobfiles, jobbytes, readbytes, joberrors,
                    jobmissingfiles, poolid, filesetid, priorjobid,
                    purgedfiles, hasbase, hascache, reviewed, comment
                ) VALUES (
                    %s, %s, %s, %s, %s, %s,
                    %s, %s, %s, %s,
                    %s, %s, %s,
                    %s, %s, %s, %s,
                    0, %s, %s, 0,
                    0, 0, 0, 0, %s
                ) RETURNING jobid
            """, (
                job_identifier, name, job_type, level, client_id, status,
                start_time, start_time, end_time, end_time,
                int(start_time.timestamp()), random.randint(1, 10000),
                int(start_time.timestamp()),
                job_files, job_bytes, job_bytes, job_errors,
                pool_id, fileset_id, comment
            ))
            job_id = self.cursor.fetchone()[0]
        else:
            self.cursor.execute("""
                INSERT INTO Job (
                    Job, Name, Type, Level, ClientId, JobStatus,
                    SchedTime, StartTime, EndTime, RealEndTime,
                    JobTDate, VolSessionId, VolSessionTime,
                    JobFiles, JobBytes, ReadBytes, JobErrors,
                    JobMissingFiles, PoolId, FileSetId, PriorJobId,
                    PurgedFiles, HasBase, HasCache, Reviewed, Comment
                ) VALUES (
                    %s, %s, %s, %s, %s, %s,
                    %s, %s, %s, %s,
                    %s, %s, %s,
                    %s, %s, %s, %s,
                    0, %s, %s, 0,
                    0, 0, 0, 0, %s
                )
            """, (
                job_identifier, name, job_type, level, client_id, status,
                start_time, start_time, end_time, end_time,
                int(start_time.timestamp()), random.randint(1, 10000),
                int(start_time.timestamp()),
                job_files, job_bytes, job_bytes, job_errors,
                pool_id, fileset_id, comment
            ))
            job_id = self.cursor.lastrowid

        self.conn.commit()
        return job_id

    def insert_job_log(self, job_id: int, log_time: datetime, log_text: str):
        """Insert a job log entry."""
        if self.db_type == 'postgresql':
            self.cursor.execute("""
                INSERT INTO log (jobid, time, logtext)
                VALUES (%s, %s, %s)
            """, (job_id, log_time, log_text))
        else:
            self.cursor.execute("""
                INSERT INTO Log (JobId, Time, LogText)
                VALUES (%s, %s, %s)
            """, (job_id, log_time, log_text))
        self.conn.commit()

    def get_or_create_path(self, path: str) -> int:
        """Get or create a path entry in the Path table."""
        # Ensure path ends with /
        if not path.endswith('/'):
            path = path + '/'

        if self.db_type == 'postgresql':
            self.cursor.execute("SELECT pathid FROM path WHERE path = %s", (path,))
        else:
            self.cursor.execute("SELECT PathId FROM Path WHERE Path = %s", (path,))

        result = self.cursor.fetchone()
        if result:
            return result[0]

        if self.db_type == 'postgresql':
            self.cursor.execute(
                "INSERT INTO path (path) VALUES (%s) RETURNING pathid",
                (path,)
            )
            path_id = self.cursor.fetchone()[0]
        else:
            self.cursor.execute(
                "INSERT INTO Path (Path) VALUES (%s)",
                (path,)
            )
            path_id = self.cursor.lastrowid

        self.conn.commit()
        return path_id

    def get_parent_path(self, path: str) -> str:
        """Get the parent path for a given path.

        For Windows paths like 'C:/Windows/System32/', returns 'C:/Windows/'
        For Linux paths like '/var/log/', returns '/var/'
        For root paths like 'C:/' or '/', returns '' (empty root)
        """
        if not path or path == '/':
            return ''  # Root has no parent

        # Remove trailing slash
        path = path.rstrip('/')

        # Check if this is a Windows drive root (e.g., 'C:')
        if len(path) == 2 and path[1] == ':':
            return ''  # Drive letter has empty root as parent

        # Find the last separator
        last_sep = path.rfind('/')
        if last_sep == -1:
            # No slash found - this might be a Windows drive letter without trailing slash
            if len(path) >= 2 and path[1] == ':':
                return ''
            return ''

        # For paths like '/var', last_sep is 0, parent is '/'
        if last_sep == 0:
            return '/'

        # For Windows paths like 'C:/Windows', last_sep is 2, parent is 'C:/'
        if last_sep == 2 and path[1] == ':':
            return path[:3]  # 'C:/'

        # Normal case: return parent with trailing slash
        return path[:last_sep] + '/'

    def build_path_hierarchy(self):
        """Build the PathHierarchy table from existing paths.

        PathHierarchy stores parent-child relationships for BVFS navigation.
        Each path entry points to its parent path via PPathId.

        For Windows: C:/Windows/System32/ -> C:/Windows/ -> C:/ -> '' (empty root)
        For Linux: /var/log/ -> /var/ -> / (root)

        Note: This should be called after all files have been generated.
        """
        print("Building PathHierarchy table for BVFS navigation...")

        # First, ensure the empty root path exists (for Windows drive letters)
        root_id = self.get_or_create_path('')

        # Get all paths
        if self.db_type == 'postgresql':
            self.cursor.execute("SELECT pathid, path FROM path")
        else:
            self.cursor.execute("SELECT PathId, Path FROM Path")

        paths = self.cursor.fetchall()
        total_paths = len(paths)
        print(f"  Processing {total_paths} paths...")

        # Clear existing hierarchy entries (for re-runs)
        if self.db_type == 'postgresql':
            self.cursor.execute("DELETE FROM pathhierarchy")
        else:
            self.cursor.execute("DELETE FROM PathHierarchy")
        self.conn.commit()

        # Build a dict of path -> pathid for quick lookup
        path_to_id = {row[1]: row[0] for row in paths}

        # Process each path and insert hierarchy entries
        batch_count = 0
        for path_id, path in paths:
            parent_path = self.get_parent_path(path)

            # Get or create parent path ID
            if parent_path in path_to_id:
                parent_id = path_to_id[parent_path]
            else:
                parent_id = self.get_or_create_path(parent_path)
                path_to_id[parent_path] = parent_id

            # Insert hierarchy entry
            if self.db_type == 'postgresql':
                self.cursor.execute("""
                    INSERT INTO pathhierarchy (pathid, ppathid)
                    VALUES (%s, %s)
                    ON CONFLICT (pathid) DO NOTHING
                """, (path_id, parent_id))
            else:
                self.cursor.execute("""
                    INSERT IGNORE INTO PathHierarchy (PathId, PPathId)
                    VALUES (%s, %s)
                """, (path_id, parent_id))

            batch_count += 1
            if batch_count >= 1000:
                self.conn.commit()
                batch_count = 0

        self.conn.commit()
        print(f"  PathHierarchy built with {total_paths} entries")

    def insert_file(self, job_id: int, path_id: int, filename: str,
                    file_size: int, mtime: datetime, mark_id: int = 0) -> int:
        """Insert a file entry into the File table.

        Note: Bareos 25.0+ stores filename directly in the file table's 'name' column.
        Older versions used a separate filename table with filenameid reference.
        """
        # LStat format: simplified - encode size and mtime
        # In real Bareos this is a base64-encoded stat structure
        mtime_ts = int(mtime.timestamp())
        lstat = f"AAA AAAA AAAA AAAA AAAA AAAA A {file_size} {mtime_ts} AAAA"
        md5 = "0" * 32  # Dummy MD5

        if self.db_type == 'postgresql':
            # Bareos 25.0+ schema: file table has 'name' column directly
            self.cursor.execute("""
                INSERT INTO file (fileindex, jobid, pathid, markid, lstat, md5, name)
                VALUES (%s, %s, %s, %s, %s, %s, %s) RETURNING fileid
            """, (random.randint(1, 999999), job_id, path_id, mark_id, lstat, md5, filename))
            file_id = self.cursor.fetchone()[0]
        else:
            # MySQL/MariaDB - also updated for Bareos 25.0+ schema
            self.cursor.execute("""
                INSERT INTO File (FileIndex, JobId, PathId, MarkId, LStat, MD5, Name)
                VALUES (%s, %s, %s, %s, %s, %s, %s)
            """, (random.randint(1, 999999), job_id, path_id, mark_id, lstat, md5, filename))
            file_id = self.cursor.lastrowid

        return file_id

    def generate_files_for_job(self, job_id: int, server_type: ServerType,
                                num_files: int, job_bytes: int, backup_time: datetime,
                                level: str = 'F') -> int:
        """Generate realistic file entries for a job based on server type.

        Args:
            job_id: The job ID to associate files with
            server_type: Type of server for realistic path/file selection
            num_files: Number of files to generate
            job_bytes: Total bytes for the job (to distribute among files)
            backup_time: Backup timestamp for mtime
            level: Backup level ('F' for full, 'I' for incremental)

        Returns:
            Number of files actually created
        """
        # Get file templates for this server type
        templates = FILE_TEMPLATES.get(server_type, DEFAULT_FILE_TEMPLATE)
        paths = templates['paths']
        file_specs = templates['files']

        # For incremental, generate fewer files
        if level == 'I':
            num_files = max(10, num_files // 10)

        # Limit max files to avoid excessive DB load (can be adjusted)
        max_files_per_job = 1000
        num_files = min(num_files, max_files_per_job)

        files_created = 0
        batch_size = 100  # Commit in batches for performance
        batch_count = 0
        total_size = 0

        for _ in range(num_files):
            # Select a random path and file
            path = random.choice(paths)
            file_spec = random.choice(file_specs)
            filename_template, min_size, max_size = file_spec

            # Generate filename with number if template has {num} or similar
            if '{num}' in filename_template:
                filename = filename_template.format(num=random.randint(1000, 9999))
            elif '{sha256}' in filename_template:
                filename = ''.join(random.choices('0123456789abcdef', k=64))
            elif '{container_id}' in filename_template:
                filename = ''.join(random.choices('0123456789abcdef', k=12)) + '-json.log'
            elif '{layer_id}' in filename_template:
                filename = ''.join(random.choices('0123456789abcdef', k=64))
            else:
                filename = filename_template

            # Randomize file size within template bounds
            file_size = random.randint(min_size, max_size)
            total_size += file_size

            # Randomize mtime within last 30 days before backup
            mtime = backup_time - timedelta(days=random.randint(0, 30),
                                            hours=random.randint(0, 23),
                                            minutes=random.randint(0, 59))

            # Get or create path ID and insert file entry
            # Note: Bareos 25.0+ stores filename directly in file table
            path_id = self.get_or_create_path(path)

            # Insert file entry with filename directly (Bareos 25.0+ schema)
            self.insert_file(job_id, path_id, filename, file_size, mtime)
            files_created += 1
            batch_count += 1

            # Commit in batches
            if batch_count >= batch_size:
                self.conn.commit()
                batch_count = 0

        # Final commit
        self.conn.commit()
        return files_created

    def generate_job_log(self, job_id: int, job_name: str, client_name: str,
                         status: str, start_time: datetime, end_time: datetime,
                         job_files: int, job_bytes: int, job_errors: int,
                         storage_name: str = "FileStorage"):
        """Generate realistic job log entries."""
        log_entries = []
        current_time = start_time
        volume = f"Vol-{storage_name}-{random.randint(1, 100):04d}"
        client_ip = f"192.168.{random.randint(1, 254)}.{random.randint(1, 254)}"
        duration = end_time - start_time

        # Format bytes for human readable
        bytes_human = self._format_bytes(job_bytes)

        # Start logs
        for template in LOG_TEMPLATES['start']:
            log_text = template.format(
                jobid=job_id, jobname=job_name, storage=storage_name, volume=volume
            )
            self.insert_job_log(job_id, current_time, log_text)
            current_time += timedelta(seconds=random.randint(1, 5))

        # Running logs (every ~10% of duration)
        num_progress_logs = random.randint(3, 8)
        time_step = duration / (num_progress_logs + 2)

        for i in range(num_progress_logs):
            current_time = start_time + time_step * (i + 1)
            files_so_far = int(job_files * (i + 1) / (num_progress_logs + 1))
            path = random.choice(BACKUP_PATHS)

            template = random.choice(LOG_TEMPLATES['running'])
            log_text = template.format(
                jobid=job_id, client=client_name, client_ip=client_ip,
                path=path, files_examined=files_so_far + random.randint(100, 1000),
                files_backed=files_so_far
            )
            self.insert_job_log(job_id, current_time, log_text)

        # Add error logs if there were errors
        if job_errors > 0:
            for _ in range(min(job_errors, 10)):  # Max 10 error log entries
                error_time = start_time + timedelta(
                    seconds=random.randint(0, int(duration.total_seconds()))
                )
                error_template = random.choice(LOG_TEMPLATES['errors'])
                path = random.choice(BACKUP_PATHS)
                log_text = f"JobId {job_id}: " + error_template.format(
                    path=path, volume=volume
                )
                self.insert_job_log(job_id, error_time, log_text)

        # End logs based on status
        current_time = end_time - timedelta(seconds=random.randint(1, 10))

        if status == self.STATUS_OK:
            templates = LOG_TEMPLATES['end_ok']
        elif status == self.STATUS_WARNING:
            templates = LOG_TEMPLATES['end_warning']
        else:
            templates = LOG_TEMPLATES['end_failed']

        for template in templates:
            log_text = template.format(
                jobid=job_id, files=job_files, bytes=job_bytes,
                bytes_human=bytes_human, endtime=end_time.strftime('%Y-%m-%d %H:%M:%S'),
                duration=str(duration), errors=job_errors,
                error_msg=random.choice(ERROR_MESSAGES) if status in [self.STATUS_FAILED, self.STATUS_ERROR] else ""
            )
            self.insert_job_log(job_id, current_time, log_text)
            current_time += timedelta(seconds=1)

    def _format_bytes(self, bytes_val: int) -> str:
        """Format bytes to human readable string."""
        for unit in ['B', 'KB', 'MB', 'GB', 'TB']:
            if abs(bytes_val) < 1024.0:
                return f"{bytes_val:.2f} {unit}"
            bytes_val /= 1024.0
        return f"{bytes_val:.2f} PB"

    def generate_backup_schedule(self, server_type: ServerType,
                                  days_back: int,
                                  use_differential: bool = False) -> List[Tuple[datetime, str]]:
        """Generate backup schedule for a server.

        Args:
            server_type: Type of server for interval calculation
            days_back: How many days of history to generate
            use_differential: If True, use Full-Differential-Incremental schedule
                             (differential mid-week, incrementals on other days)
        """
        schedule = []
        now = datetime.now()

        # Determine intervals based on server type
        if server_type in [ServerType.LINUX_DB, ServerType.WINDOWS_DB,
                           ServerType.POSTGRES_DB, ServerType.MSSQL_DB,
                           ServerType.ORACLE_DB, ServerType.MONGODB]:
            full_interval = 1  # Daily full for databases
            incr_interval = 6  # Hours
            diff_day = None  # No differential for frequent fulls
        elif server_type in [ServerType.NAS_SYNOLOGY, ServerType.NAS_QNAP,
                             ServerType.NAS_NETAPP]:
            full_interval = 7  # Weekly full
            incr_interval = 24
            diff_day = 3  # Wednesday (0=Monday)
        elif server_type in [ServerType.VMWARE_HOST, ServerType.HYPERV_HOST]:
            full_interval = 7
            incr_interval = 24
            diff_day = 3  # Wednesday
        elif server_type in [ServerType.MAIL_EXCHANGE, ServerType.MAIL_LINUX]:
            full_interval = 7
            incr_interval = 12
            diff_day = 3  # Wednesday
        else:
            full_interval = 7  # Weekly full
            incr_interval = 24  # Daily incremental
            diff_day = 3  # Wednesday

        current = now - timedelta(days=days_back)
        last_full = None

        while current < now:
            if last_full is None or (current - last_full).days >= full_interval:
                schedule.append((current, self.LEVEL_FULL))
                last_full = current
            elif use_differential and diff_day is not None and current.weekday() == diff_day:
                # Differential on specified day (e.g., Wednesday)
                schedule.append((current, self.LEVEL_DIFFERENTIAL))
            else:
                schedule.append((current, self.LEVEL_INCREMENTAL))
            current += timedelta(hours=incr_interval)

        return schedule

    def generate_always_incremental_schedule(self, server_type: ServerType,
                                              days_back: int,
                                              consolidate_interval_days: int = 7
                                              ) -> Tuple[List[Tuple[datetime, str]], List[datetime]]:
        """Generate Always Incremental backup schedule with consolidate jobs.

        Always Incremental strategy:
        - One initial Full backup at the beginning
        - All subsequent backups are Incremental
        - Consolidate jobs run periodically to merge incrementals into Virtual Full

        Args:
            server_type: Type of server for interval calculation
            days_back: How many days of history to generate
            consolidate_interval_days: Days between consolidate jobs (default: 7)

        Returns:
            Tuple of (backup_schedule, consolidate_times)
            - backup_schedule: List of (datetime, level) for backup jobs
            - consolidate_times: List of datetimes for consolidate jobs
        """
        backup_schedule = []
        consolidate_times = []
        now = datetime.now()

        # Determine incremental interval based on server type
        if server_type in [ServerType.LINUX_DB, ServerType.WINDOWS_DB,
                           ServerType.POSTGRES_DB, ServerType.MSSQL_DB,
                           ServerType.ORACLE_DB, ServerType.MONGODB]:
            incr_interval_hours = 6  # Every 6 hours for databases
        elif server_type in [ServerType.MAIL_EXCHANGE, ServerType.MAIL_LINUX]:
            incr_interval_hours = 12  # Every 12 hours for mail
        else:
            incr_interval_hours = 24  # Daily for others

        current = now - timedelta(days=days_back)
        initial_full_done = False
        last_consolidate = current

        while current < now:
            if not initial_full_done:
                # First backup is always Full
                backup_schedule.append((current, self.LEVEL_FULL))
                initial_full_done = True
                last_consolidate = current
            else:
                # All subsequent backups are Incremental
                backup_schedule.append((current, self.LEVEL_INCREMENTAL))

            # Check if we need a consolidate job (after consolidate_interval_days)
            if initial_full_done and (current - last_consolidate).days >= consolidate_interval_days:
                # Consolidate job runs after backups, typically early morning
                consolidate_time = current.replace(hour=3, minute=0, second=0)
                if consolidate_time > last_consolidate:
                    consolidate_times.append(consolidate_time)
                    last_consolidate = current

            current += timedelta(hours=incr_interval_hours)

        return backup_schedule, consolidate_times

    def generate_consolidate_job(self, client_id: int, client_name: str,
                                  pool_id: int, fileset_id: int,
                                  server_type: ServerType,
                                  schedule_time: datetime,
                                  storage_name: str = "FileStorage",
                                  generate_logs: bool = True,
                                  success_rate: Optional[float] = None) -> Tuple[int, str]:
        """Generate a consolidate job (Virtual Full from incrementals).

        Consolidate jobs merge multiple incremental backups into a single
        Virtual Full backup, reducing the backup chain length.

        Args:
            client_id: Client ID
            client_name: Client name
            pool_id: Pool ID
            fileset_id: FileSet ID
            server_type: Server type for realistic data
            schedule_time: When the job ran
            storage_name: Storage name
            generate_logs: Whether to generate log entries
            success_rate: Override success rate (0.0-1.0)

        Returns:
            Tuple of (job_id, status)
        """
        profile = SERVER_PROFILES.get(server_type)
        if not profile:
            profile = SERVER_PROFILES[ServerType.LINUX_APP]

        # Consolidate jobs have high success rate (they're internal operations)
        if success_rate is not None:
            actual_failure_rate = (1.0 - success_rate) * 0.3  # Lower failure rate
            actual_warning_rate = (1.0 - success_rate) * 0.2
        else:
            actual_failure_rate = profile.failure_rate * 0.5  # Half the normal rate
            actual_warning_rate = profile.warning_rate * 0.5

        # Determine status
        rand = random.random()
        if rand < actual_failure_rate:
            status = random.choice([self.STATUS_FAILED, self.STATUS_ERROR])
            job_errors = random.randint(1, 10)
            job_files = 0
            job_bytes = 0
        elif rand < actual_failure_rate + actual_warning_rate:
            status = self.STATUS_WARNING
            job_errors = random.randint(1, 5)
            # Consolidate produces Virtual Full - similar size to Full
            job_files = random.randint(profile.files_min, profile.files_max)
            size_gb = random.uniform(profile.size_gb_min, profile.size_gb_max)
            job_bytes = int(size_gb * 1024 * 1024 * 1024)
        else:
            status = self.STATUS_OK
            job_errors = 0
            job_files = random.randint(profile.files_min, profile.files_max)
            size_gb = random.uniform(profile.size_gb_min, profile.size_gb_max)
            job_bytes = int(size_gb * 1024 * 1024 * 1024)

        # Consolidate jobs take longer than regular backups (reading + writing)
        duration_hours = random.uniform(
            profile.duration_hours_min * 1.5,
            profile.duration_hours_max * 2.0
        )
        end_time = schedule_time + timedelta(hours=duration_hours)

        job_name = f"Consolidate-{client_name}"

        job_id = self.insert_job(
            name=job_name,
            client_id=client_id,
            pool_id=pool_id,
            fileset_id=fileset_id,
            job_type=self.TYPE_CONSOLIDATE,
            level=self.LEVEL_VIRTUAL_FULL,
            status=status,
            start_time=schedule_time,
            end_time=end_time,
            job_files=job_files,
            job_bytes=job_bytes,
            job_errors=job_errors
        )

        if generate_logs:
            self.generate_consolidate_job_log(
                job_id=job_id,
                job_name=job_name,
                client_name=client_name,
                status=status,
                start_time=schedule_time,
                end_time=end_time,
                job_files=job_files,
                job_bytes=job_bytes,
                job_errors=job_errors,
                storage_name=storage_name
            )

        return job_id, status

    def generate_consolidate_job_log(self, job_id: int, job_name: str, client_name: str,
                                      status: str, start_time: datetime, end_time: datetime,
                                      job_files: int, job_bytes: int, job_errors: int,
                                      storage_name: str = "FileStorage"):
        """Generate log entries for a consolidate job."""
        current_time = start_time
        bytes_human = self._format_bytes(job_bytes)
        volume = f"Vol-{storage_name}-{random.randint(1, 100):04d}"

        # Start logs
        self.insert_job_log(job_id, current_time,
            f"JobId {job_id}: Start Consolidate JobId {job_id}, Job={job_name}")
        current_time += timedelta(seconds=random.randint(1, 3))

        self.insert_job_log(job_id, current_time,
            f"JobId {job_id}: Using Device \"{storage_name}\" to write.")
        current_time += timedelta(seconds=random.randint(1, 3))

        self.insert_job_log(job_id, current_time,
            f"JobId {job_id}: Consolidating incrementals for client {client_name}.")
        current_time += timedelta(seconds=random.randint(1, 3))

        # Progress logs
        duration = end_time - start_time
        num_progress = random.randint(3, 6)
        incr_count = random.randint(5, 20)

        for i in range(num_progress):
            progress_time = start_time + (duration * (i + 1) / (num_progress + 1))
            processed = int(incr_count * (i + 1) / (num_progress + 1))
            self.insert_job_log(job_id, progress_time,
                f"JobId {job_id}: Processed {processed} of {incr_count} incremental backups.")

        # Error logs if any
        if job_errors > 0:
            for _ in range(min(job_errors, 5)):
                error_time = start_time + timedelta(
                    seconds=random.randint(0, int(duration.total_seconds()))
                )
                error_msg = random.choice([
                    "Warning: Volume nearly full during consolidation.",
                    "Retrying read operation on incremental volume.",
                    "Minor checksum warning, continuing.",
                ])
                self.insert_job_log(job_id, error_time, f"JobId {job_id}: {error_msg}")

        # End logs
        current_time = end_time - timedelta(seconds=random.randint(1, 10))

        if status == self.STATUS_OK:
            self.insert_job_log(job_id, current_time,
                f"JobId {job_id}: Consolidate OK. Virtual Full created: Files={job_files} Bytes={job_bytes} ({bytes_human})")
            current_time += timedelta(seconds=1)
            self.insert_job_log(job_id, current_time,
                f"JobId {job_id}: Consolidation completed successfully.")
        elif status == self.STATUS_WARNING:
            self.insert_job_log(job_id, current_time,
                f"JobId {job_id}: Consolidate completed with {job_errors} warning(s).")
            current_time += timedelta(seconds=1)
            self.insert_job_log(job_id, current_time,
                f"JobId {job_id}: Virtual Full created: Files={job_files} Bytes={bytes_human}")
        else:
            self.insert_job_log(job_id, current_time,
                f"JobId {job_id}: Consolidate FAILED. Could not create Virtual Full.")
            current_time += timedelta(seconds=1)
            self.insert_job_log(job_id, current_time,
                f"JobId {job_id}: Error: {random.choice(['Storage full', 'Connection lost', 'Timeout during consolidation'])}")

        self.insert_job_log(job_id, end_time,
            f"JobId {job_id}: Terminating at {end_time.strftime('%Y-%m-%d %H:%M:%S')}")

    def generate_job(self, client_id: int, client_name: str,
                     pool_id: int, fileset_id: int,
                     server_type: ServerType,
                     schedule_time: datetime, level: str,
                     storage_name: str = "FileStorage",
                     generate_logs: bool = True,
                     generate_files: bool = False,
                     success_rate: Optional[float] = None,
                     warning_rate: Optional[float] = None,
                     failure_rate: Optional[float] = None) -> Tuple[int, str]:
        """Generate a single backup job with optional logs and file entries.

        Args:
            generate_logs: Generate job log entries
            generate_files: Generate file entries in Path/Filename/File tables
            success_rate: Override success rate (0.0-1.0). If set, other rates are calculated.
            warning_rate: Override warning rate (0.0-1.0)
            failure_rate: Override failure rate (0.0-1.0)
        """
        profile = SERVER_PROFILES.get(server_type)
        if not profile:
            profile = SERVER_PROFILES[ServerType.LINUX_APP]

        # Use custom rates if provided, otherwise use profile defaults
        if success_rate is not None:
            # If success_rate is set, calculate others proportionally
            actual_failure_rate = (1.0 - success_rate) * 0.6  # 60% of failures are actual failures
            actual_warning_rate = (1.0 - success_rate) * 0.4  # 40% are warnings
        else:
            actual_failure_rate = failure_rate if failure_rate is not None else profile.failure_rate
            actual_warning_rate = warning_rate if warning_rate is not None else profile.warning_rate

        # Determine status
        rand = random.random()
        if rand < actual_failure_rate:
            status = random.choice([self.STATUS_FAILED, self.STATUS_ERROR, self.STATUS_CANCELED])
            job_errors = random.randint(1, 50)
            job_files = random.randint(0, profile.files_min // 10)
            job_bytes = job_files * random.randint(100, 5000)
        elif rand < actual_failure_rate + actual_warning_rate:
            status = self.STATUS_WARNING
            job_errors = random.randint(1, 10)
            job_files = random.randint(profile.files_min, profile.files_max)
            size_gb = random.uniform(profile.size_gb_min, profile.size_gb_max)
            if level == self.LEVEL_INCREMENTAL:
                size_gb *= profile.incr_ratio
            elif level == self.LEVEL_DIFFERENTIAL:
                # Differential is larger than incremental (avg of 3-4 incrementals)
                size_gb *= profile.incr_ratio * random.uniform(2.5, 4.0)
            job_bytes = int(size_gb * 1024 * 1024 * 1024)
        else:
            status = self.STATUS_OK
            job_errors = 0
            job_files = random.randint(profile.files_min, profile.files_max)
            size_gb = random.uniform(profile.size_gb_min, profile.size_gb_max)
            if level == self.LEVEL_INCREMENTAL:
                size_gb *= profile.incr_ratio
                job_files = int(job_files * profile.incr_ratio)
            elif level == self.LEVEL_DIFFERENTIAL:
                # Differential is larger than incremental (avg of 3-4 incrementals)
                diff_ratio = profile.incr_ratio * random.uniform(2.5, 4.0)
                size_gb *= diff_ratio
                job_files = int(job_files * diff_ratio)
            job_bytes = int(size_gb * 1024 * 1024 * 1024)

        # Calculate duration
        duration = random.uniform(profile.duration_hours_min, profile.duration_hours_max)
        if level == self.LEVEL_INCREMENTAL:
            duration *= profile.incr_ratio
        elif level == self.LEVEL_DIFFERENTIAL:
            # Differential takes longer than incremental
            duration *= profile.incr_ratio * random.uniform(2.0, 3.0)

        start_time = schedule_time + timedelta(minutes=random.randint(0, 20))
        end_time = start_time + timedelta(hours=duration)

        job_name = f"Backup-{client_name}"

        job_id = self.insert_job(
            name=job_name,
            client_id=client_id,
            pool_id=pool_id,
            fileset_id=fileset_id,
            job_type=self.TYPE_BACKUP,
            level=level,
            status=status,
            start_time=start_time,
            end_time=end_time,
            job_files=job_files,
            job_bytes=job_bytes,
            job_errors=job_errors
        )

        # Generate job logs
        if generate_logs:
            self.generate_job_log(
                job_id=job_id,
                job_name=job_name,
                client_name=client_name,
                status=status,
                start_time=start_time,
                end_time=end_time,
                job_files=job_files,
                job_bytes=job_bytes,
                job_errors=job_errors,
                storage_name=storage_name
            )

        # Generate file entries
        if generate_files and status in [self.STATUS_OK, self.STATUS_WARNING]:
            self.generate_files_for_job(
                job_id=job_id,
                server_type=server_type,
                num_files=job_files,
                job_bytes=job_bytes,
                backup_time=start_time,
                level=level
            )

        return job_id, status

    def generate_small_company(self, days_back: int, generate_logs: bool = True,
                                generate_files: bool = False,
                                success_rate: Optional[float] = None,
                                always_incremental: bool = False,
                                consolidate_interval: int = 7):
        """Generate test data for a small company."""
        print("=" * 60)
        print(f"Generating: {SMALL_COMPANY['name']}")
        print("=" * 60)
        print(f"Servers: {len(SMALL_COMPANY['servers'])}")
        print(f"History: {days_back} days")
        if success_rate is not None:
            print(f"Success Rate: {success_rate * 100:.0f}%")
        if generate_files:
            print("File Generation: Enabled")
        if always_incremental:
            print(f"Strategy: Always Incremental (Consolidate every {consolidate_interval} days)")

        return self._generate_company_data(
            SMALL_COMPANY['servers'],
            SMALL_COMPANY['locations'],
            days_back,
            generate_logs,
            generate_files,
            success_rate,
            always_incremental,
            consolidate_interval
        )

    def generate_medium_company(self, days_back: int, generate_logs: bool = True,
                                 generate_files: bool = False,
                                 success_rate: Optional[float] = None,
                                 always_incremental: bool = False,
                                 consolidate_interval: int = 7):
        """Generate test data for a medium company."""
        print("=" * 60)
        print(f"Generating: {MEDIUM_COMPANY['name']}")
        print("=" * 60)
        print(f"Servers: {len(MEDIUM_COMPANY['servers'])}")
        print(f"History: {days_back} days")
        if success_rate is not None:
            print(f"Success Rate: {success_rate * 100:.0f}%")
        if generate_files:
            print("File Generation: Enabled")
        if always_incremental:
            print(f"Strategy: Always Incremental (Consolidate every {consolidate_interval} days)")

        return self._generate_company_data(
            MEDIUM_COMPANY['servers'],
            MEDIUM_COMPANY['locations'],
            days_back,
            generate_logs,
            generate_files,
            success_rate,
            always_incremental,
            consolidate_interval
        )

    def generate_large_company(self, days_back: int, generate_logs: bool = True,
                                generate_files: bool = False,
                                success_rate: Optional[float] = None,
                                always_incremental: bool = False,
                                consolidate_interval: int = 7):
        """Generate test data for a large enterprise."""
        print("=" * 60)
        print(f"Generating: {LARGE_COMPANY['name']}")
        print("=" * 60)
        print(f"Locations: {len(LARGE_COMPANY['locations'])}")
        print(f"History: {days_back} days")
        if success_rate is not None:
            print(f"Success Rate: {success_rate * 100:.0f}%")
        if generate_files:
            print("File Generation: Enabled")
        if always_incremental:
            print(f"Strategy: Always Incremental (Consolidate every {consolidate_interval} days)")

        # Build server list from templates
        servers = []
        for location in LARGE_COMPANY['locations']:
            for server_type, prefix, count in LARGE_COMPANY['server_templates']:
                for i in range(count):
                    name = f"{prefix}-{location.code.lower()}-{i+1:02d}"
                    desc = f"{server_type.value} at {location.name}"
                    servers.append(ServerConfig(name, server_type, desc))

        # Add cloud servers
        servers.extend(LARGE_COMPANY['cloud_servers'])

        print(f"Total Servers: {len(servers)}")

        return self._generate_company_data(
            servers,
            LARGE_COMPANY['locations'],
            days_back,
            generate_logs,
            generate_files,
            success_rate,
            always_incremental,
            consolidate_interval
        )

    def _generate_company_data(self, servers: List[ServerConfig],
                                locations: List[Location],
                                days_back: int,
                                generate_logs: bool,
                                generate_files: bool = False,
                                success_rate: Optional[float] = None,
                                always_incremental: bool = False,
                                consolidate_interval: int = 7) -> Dict:
        """Generate company data from server list.

        Args:
            servers: List of server configurations
            locations: List of locations
            days_back: Days of backup history to generate
            generate_logs: Whether to generate job logs
            generate_files: Whether to generate file entries
            success_rate: Override success rate (0.0-1.0)
            always_incremental: Use Always Incremental strategy with Consolidate jobs
            consolidate_interval: Days between consolidate jobs (default: 7)
        """
        # Create pools
        pools = {
            'full': self.get_or_create_pool("TestData-Full"),
            'incr': self.get_or_create_pool("TestData-Incremental"),
        }
        if always_incremental:
            pools['consolidate'] = self.get_or_create_pool("TestData-Consolidate")

        # Create filesets
        filesets = {
            'windows': self.get_or_create_fileset("Windows-Standard"),
            'linux': self.get_or_create_fileset("Linux-Standard"),
            'database': self.get_or_create_fileset("Database-Full"),
            'nas': self.get_or_create_fileset("NAS-Data"),
            'vm': self.get_or_create_fileset("VM-Image"),
            'cloud': self.get_or_create_fileset("Cloud-Sync"),
        }

        # Create storage
        storage_id = self.get_or_create_storage("FileStorage")

        # Statistics
        total_jobs = 0
        total_logs = 0
        stats_by_status = {
            self.STATUS_OK: 0,
            self.STATUS_WARNING: 0,
            self.STATUS_FAILED: 0,
            self.STATUS_ERROR: 0,
            self.STATUS_CANCELED: 0,
        }

        print("\n" + "-" * 60)
        print("Generating backup jobs and logs...")
        print("-" * 60)

        for server in servers:
            client_id = self.create_client(server.name, server.description)
            client_name = f"{server.name}-fd"

            # Select pool and fileset based on server type
            if server.server_type in [ServerType.LINUX_DB, ServerType.WINDOWS_DB,
                                       ServerType.POSTGRES_DB, ServerType.MSSQL_DB,
                                       ServerType.ORACLE_DB, ServerType.MONGODB,
                                       ServerType.SAP_HANA]:
                pool_id = pools['full']
                fileset_id = filesets['database']
            elif server.server_type in [ServerType.NAS_SYNOLOGY, ServerType.NAS_QNAP,
                                         ServerType.NAS_NETAPP]:
                pool_id = pools['full']
                fileset_id = filesets['nas']
            elif server.server_type in [ServerType.VMWARE_HOST, ServerType.HYPERV_HOST]:
                pool_id = pools['full']
                fileset_id = filesets['vm']
            elif server.server_type in [ServerType.CLOUD_AWS, ServerType.CLOUD_AZURE,
                                         ServerType.CLOUD_GCP]:
                pool_id = pools['incr']
                fileset_id = filesets['cloud']
            elif "LINUX" in server.server_type.name:
                pool_id = pools['incr']
                fileset_id = filesets['linux']
            else:
                pool_id = pools['incr']
                fileset_id = filesets['windows']

            # Generate schedule and jobs
            jobs_created = 0
            consolidate_jobs = 0

            if always_incremental:
                # Always Incremental strategy with Consolidate jobs
                backup_schedule, consolidate_times = self.generate_always_incremental_schedule(
                    server.server_type, days_back, consolidate_interval
                )

                # Generate backup jobs
                for sched_time, level in backup_schedule:
                    job_id, status = self.generate_job(
                        client_id, client_name,
                        pool_id, fileset_id,
                        server.server_type, sched_time, level,
                        generate_logs=generate_logs,
                        generate_files=generate_files,
                        success_rate=success_rate
                    )
                    jobs_created += 1
                    total_jobs += 1
                    if generate_logs:
                        total_logs += random.randint(10, 25)
                    if status in stats_by_status:
                        stats_by_status[status] += 1

                # Generate consolidate jobs
                for consolidate_time in consolidate_times:
                    job_id, status = self.generate_consolidate_job(
                        client_id, client_name,
                        pools['consolidate'], fileset_id,
                        server.server_type, consolidate_time,
                        generate_logs=generate_logs,
                        success_rate=success_rate
                    )
                    consolidate_jobs += 1
                    total_jobs += 1
                    if generate_logs:
                        total_logs += random.randint(8, 15)
                    if status in stats_by_status:
                        stats_by_status[status] += 1
            else:
                # Traditional backup strategy (Full + Differential + Incremental)
                # Use differential for server types with weekly full backups
                use_diff = server.server_type in [
                    ServerType.NAS_SYNOLOGY, ServerType.NAS_QNAP, ServerType.NAS_NETAPP,
                    ServerType.VMWARE_HOST, ServerType.HYPERV_HOST,
                    ServerType.MAIL_EXCHANGE, ServerType.MAIL_LINUX,
                    ServerType.LINUX_APP, ServerType.LINUX_WEB, ServerType.LINUX_DB,
                    ServerType.WINDOWS_APP, ServerType.WINDOWS_FILE, ServerType.WINDOWS_DB,
                ]
                schedule = self.generate_backup_schedule(server.server_type, days_back, use_differential=use_diff)

                for sched_time, level in schedule:
                    job_id, status = self.generate_job(
                        client_id, client_name,
                        pool_id, fileset_id,
                        server.server_type, sched_time, level,
                        generate_logs=generate_logs,
                        generate_files=generate_files,
                        success_rate=success_rate
                    )
                    jobs_created += 1
                    total_jobs += 1
                    if generate_logs:
                        total_logs += random.randint(10, 25)
                    if status in stats_by_status:
                        stats_by_status[status] += 1

            if consolidate_jobs > 0:
                print(f"  {server.name:20} ({server.server_type.value:30}): {jobs_created:4} jobs + {consolidate_jobs} consolidate")
            else:
                print(f"  {server.name:20} ({server.server_type.value:30}): {jobs_created:4} jobs")

        # Summary
        print("\n" + "=" * 60)
        print("SUMMARY")
        print("=" * 60)
        print(f"\nTotal servers: {len(servers)}")
        print(f"Total jobs:    {total_jobs}")
        if generate_logs:
            print(f"Total logs:    ~{total_logs} entries")

        print("\nJobs by status:")
        status_names = {
            self.STATUS_OK: "OK",
            self.STATUS_WARNING: "Warning",
            self.STATUS_FAILED: "Failed",
            self.STATUS_ERROR: "Error",
            self.STATUS_CANCELED: "Canceled",
        }
        for status, count in stats_by_status.items():
            pct = (count / total_jobs * 100) if total_jobs > 0 else 0
            print(f"  {status_names.get(status, status):10}: {count:4} ({pct:.1f}%)")

        # Build PathHierarchy table for BVFS navigation (only if files were generated)
        if generate_files:
            print("\n" + "-" * 60)
            self.build_path_hierarchy()

        print("\n" + "=" * 60)
        print("Test data generation complete!")
        print("=" * 60)

        return {
            'servers': len(servers),
            'jobs': total_jobs,
            'stats': stats_by_status
        }

    def generate_config_files(self, servers: List[ServerConfig],
                               output_dir: str,
                               always_incremental: bool = False,
                               consolidate_interval: int = 7):
        """Generate Bareos configuration files matching the test data.

        Args:
            servers: List of server configurations
            output_dir: Directory to write configuration files
            always_incremental: Use Always Incremental strategy
            consolidate_interval: Days between consolidate jobs

        Creates subdirectories:
            - client/      - Client definitions
            - job/         - Job definitions
            - fileset/     - FileSet definitions
            - pool/        - Pool definitions
            - schedule/    - Schedule definitions
            - storage/     - Storage definitions
            - console/     - Console definitions
        """
        import os as os_module

        # Create output directories
        dirs = ['client', 'job', 'fileset', 'pool', 'schedule', 'storage', 'console']
        for d in dirs:
            path = os_module.path.join(output_dir, d)
            os_module.makedirs(path, exist_ok=True)

        print(f"\nGenerating configuration files in: {output_dir}")
        print("-" * 60)

        # Generate pools
        pools = [
            ('TestData-Full', 365, '50G', 100, 'Full'),
            ('TestData-Incremental', 180, '10G', 500, 'Incr'),
        ]
        if always_incremental:
            pools.append(('TestData-Consolidate', 365, '100G', 50, 'Consolidate'))

        for pool_name, retention, max_bytes, max_vols, label_prefix in pools:
            pool_content = CONFIG_TEMPLATES['pool'].format(
                name=pool_name,
                retention=retention,
                max_bytes=max_bytes,
                max_vols=max_vols,
                label_prefix=label_prefix
            )
            pool_file = os_module.path.join(output_dir, 'pool', f'{pool_name}.conf')
            with open(pool_file, 'w') as f:
                f.write(pool_content)
        print(f"  Created {len(pools)} pool configurations")

        # Generate filesets
        filesets = [
            ('Linux-Standard', 'fileset_linux'),
            ('Windows-Standard', 'fileset_windows'),
            ('Database-Full', 'fileset_database'),
        ]
        for fs_name, template in filesets:
            fs_content = CONFIG_TEMPLATES[template].format(name=fs_name)
            fs_file = os_module.path.join(output_dir, 'fileset', f'{fs_name}.conf')
            with open(fs_file, 'w') as f:
                f.write(fs_content)
        print(f"  Created {len(filesets)} fileset configurations")

        # Generate schedules
        schedules = []
        if always_incremental:
            schedules.append(('AlwaysIncremental-Weekly', 'schedule_always_incremental'))
        else:
            schedules.append(('FullWeekly-IncrDaily', 'schedule_full_weekly'))
            schedules.append(('FullDaily-IncrHourly', 'schedule_full_daily'))

        for sched_name, template in schedules:
            if template == 'schedule_always_incremental':
                sched_content = CONFIG_TEMPLATES[template].format(
                    name=sched_name,
                    full_pool='TestData-Full',
                    incr_pool='TestData-Incremental',
                    client_base='template',
                    fileset='Linux-Standard',
                    consolidate_schedule=f'{sched_name}-Consolidate',
                    consolidate_pool='TestData-Consolidate',
                    consolidate_interval=consolidate_interval
                )
            else:
                sched_content = CONFIG_TEMPLATES[template].format(
                    name=sched_name,
                    full_pool='TestData-Full',
                    incr_pool='TestData-Incremental'
                )
            sched_file = os_module.path.join(output_dir, 'schedule', f'{sched_name}.conf')
            with open(sched_file, 'w') as f:
                f.write(sched_content)
        print(f"  Created {len(schedules)} schedule configurations")

        # Generate storage
        storage_content = CONFIG_TEMPLATES['storage'].format(
            name='FileStorage',
            address='localhost',
            password='TestStoragePassword123'
        )
        storage_file = os_module.path.join(output_dir, 'storage', 'FileStorage.conf')
        with open(storage_file, 'w') as f:
            f.write(storage_content)
        print("  Created storage configuration")

        # Generate console for testing
        console_configs = [
            ('onesimus', 'OneSimusPassword123', 'yes', 'yes'),
            ('test-psk', 'TestPSKPassword123', 'yes', 'yes'),
            ('test-legacy', 'TestLegacyPassword123', 'no', 'no'),
        ]
        for console_name, password, tls_enable, tls_require in console_configs:
            console_content = CONFIG_TEMPLATES['console'].format(
                name=console_name,
                password=password,
                tls_enable=tls_enable,
                tls_require=tls_require
            )
            console_file = os_module.path.join(output_dir, 'console', f'{console_name}.conf')
            with open(console_file, 'w') as f:
                f.write(console_content)
        print(f"  Created {len(console_configs)} console configurations")

        # Generate clients and jobs
        client_count = 0
        job_count = 0

        for server in servers:
            # Generate random password and address
            password = f"TestPassword{random.randint(1000, 9999)}"
            address = f"192.168.{random.randint(1, 254)}.{random.randint(1, 254)}"

            # Determine fileset based on server type
            if server.server_type in [ServerType.LINUX_DB, ServerType.WINDOWS_DB,
                                       ServerType.POSTGRES_DB, ServerType.MSSQL_DB,
                                       ServerType.ORACLE_DB, ServerType.MONGODB]:
                fileset = 'Database-Full'
                file_retention = 60
                job_retention = 180
            elif "LINUX" in server.server_type.name:
                fileset = 'Linux-Standard'
                file_retention = 30
                job_retention = 90
            else:
                fileset = 'Windows-Standard'
                file_retention = 30
                job_retention = 90

            # Generate client config
            client_content = CONFIG_TEMPLATES['client'].format(
                name=server.name,
                address=address,
                password=password,
                file_retention=file_retention,
                job_retention=job_retention,
                description=server.description
            )
            client_file = os_module.path.join(output_dir, 'client', f'{server.name}-fd.conf')
            with open(client_file, 'w') as f:
                f.write(client_content)
            client_count += 1

            # Determine schedule
            if server.server_type in [ServerType.LINUX_DB, ServerType.WINDOWS_DB,
                                       ServerType.POSTGRES_DB, ServerType.MSSQL_DB]:
                if always_incremental:
                    schedule = 'AlwaysIncremental-Weekly'
                else:
                    schedule = 'FullDaily-IncrHourly'
                pool = 'TestData-Full'
            else:
                if always_incremental:
                    schedule = 'AlwaysIncremental-Weekly'
                else:
                    schedule = 'FullWeekly-IncrDaily'
                pool = 'TestData-Incremental'

            # Generate job config
            job_content = CONFIG_TEMPLATES['job'].format(
                client_name=server.name,
                level='Full',
                fileset=fileset,
                schedule=schedule,
                pool=pool,
                description=server.description
            )
            job_file = os_module.path.join(output_dir, 'job', f'Backup-{server.name}.conf')
            with open(job_file, 'w') as f:
                f.write(job_content)
            job_count += 1

        print(f"  Created {client_count} client configurations")
        print(f"  Created {job_count} job configurations")

        print("-" * 60)
        print(f"Configuration files generated in: {output_dir}")
        print("These files can be included in your bareos-dir.conf with:")
        print(f"  @|\"sh -c 'for f in {output_dir}/*/*.conf; do echo @$f; done'\"")

    def cleanup_test_data(self):
        """Remove all test data created by this script."""
        print("Cleaning up test data...")

        # Delete job logs first (foreign key)
        self.cursor.execute("""
            DELETE FROM log
            WHERE jobid IN (
                SELECT jobid FROM job
                WHERE comment LIKE %s
            )
        """, (f"%{self.script_marker}%",))
        deleted_logs = self.cursor.rowcount
        print(f"Deleted {deleted_logs} log entries")

        # Delete jobs
        self.cursor.execute("""
            DELETE FROM job
            WHERE comment LIKE %s
        """, (f"%{self.script_marker}%",))
        deleted_jobs = self.cursor.rowcount
        print(f"Deleted {deleted_jobs} jobs")

        # Delete test clients
        self.cursor.execute("""
            DELETE FROM client
            WHERE name LIKE '%-fd'
            AND uname LIKE '%Test%' OR uname LIKE '%Server%' OR uname LIKE '%at %'
        """)
        deleted_clients = self.cursor.rowcount
        print(f"Deleted {deleted_clients} clients")

        self.conn.commit()
        print("Cleanup complete!")


# =============================================================================
# Help Text
# =============================================================================

HELP_SHORT = """
Bareos Testdaten-Generator - Kurzübersicht
==========================================

Verwendung:
  python generate_bareos_testdata.py -s <small|medium|large> [OPTIONEN]

Unternehmensgrößen:
  small   - Kleine Firma (~6 Server, 30 Tage)
  medium  - Mittelstand (~12 Server, 60 Tage)
  large   - Enterprise (~50-200 Server, 90 Tage)

Wichtigste Parameter:
  -s, --size GRÖSSE        Unternehmensgröße (default: medium)
  -r, --success-rate %     Erfolgsrate der Jobs (0-100)
  -d, --days TAGE          Backup-Historie in Tagen
  -a, --always-incremental Always Incremental mit Consolidate Jobs
  -i, --consolidate-interval  Tage zwischen Consolidate Jobs (default: 7)
  -n, --no-logs            Ohne Job-Logs (schneller)
  -c, --cleanup            Testdaten löschen

Konfigurationsdateien:
  -g, --generate-configs DIR  Bareos-Konfigurationen generieren
  --configs-only              Nur Konfigurationen (keine DB benötigt)

Datenbank-Parameter:
  -t, --db-type TYPE       postgresql oder mysql
  -H, --host HOST          Datenbank-Host (default: localhost)
  -P, --port PORT          Datenbank-Port
  -D, --database NAME      Datenbankname (default: bareos)
  -u, --user USER          Datenbank-Benutzer
  -p, --password PASS      Datenbank-Passwort

Kurzbeispiele:
  python generate_bareos_testdata.py -s medium -t postgresql -u bareos -p geheim
  python generate_bareos_testdata.py -s large -r 95 -d 180
  python generate_bareos_testdata.py -s medium -a -i 14   # Always Incremental
  python generate_bareos_testdata.py --configs-only -s medium -g ./test-configs
  python generate_bareos_testdata.py -c

Ausführliche Hilfe:
  python generate_bareos_testdata.py -h -h
  python generate_bareos_testdata.py --detailed-help
"""

HELP_DETAILED = """
================================================================================
              BAREOS TESTDATEN-GENERATOR - AUSFÜHRLICHE DOKUMENTATION
================================================================================

ÜBERSICHT
---------
Generiert realistische Backup-Testdaten für die Bareos/Bacula-Datenbank.
Erstellt Clients, Jobs, Job-Logs, Pools und Filesets.

================================================================================
UNTERNEHMENSGRÖSZEN IM DETAIL
================================================================================

SMALL - Kleine Firma GmbH (~6 Server)
-------------------------------------
  Server:
    dc01     Windows Domain Controller    Primary Active Directory
    dc02     Windows Domain Controller    Secondary Active Directory
    fs01     Windows File Server          Dateiablage
    web01    Linux Web Server             Webserver
    db01     Linux Database Server        PostgreSQL/MySQL
    nas01    Synology NAS                 Backup-Speicher

  Backup-Schema:
    - Domain Controller: Wöchentlich Full, täglich Incremental
    - Datenbanken: Täglich Full, alle 6h Incremental
    - Andere: Wöchentlich Full, täglich Incremental

MEDIUM - Mittelstand AG (~12 Server)
------------------------------------
  Server:
    dc01, dc02   Windows Domain Controller   Primary + Secondary DC
    fs01         Windows File Server         Hauptdateiserver
    nas01        Synology NAS                NAS-Speicher
    app01        Windows Application         ERP-System
    app02        Windows Application         CRM-Anwendung
    web01        Linux Web Server            Unternehmenswebsite
    web02        Linux Web Server            Intranet-Portal
    db01         Linux Database              PostgreSQL
    sql01        Windows Database            MS SQL Server
    esx01, esx02 VMware ESXi Host            Virtualisierung

LARGE - Enterprise Konzern GmbH (~50-200 Server)
------------------------------------------------
  Standorte:
    FRA    Frankfurt Datacenter
    MUC    München Office
    BER    Berlin Office
    LON    London Office
    AWS-EU AWS eu-central-1
    AZ-WE  Azure West Europe

  Server pro Standort (ca. 20 pro Standort):
    2x Domain Controller, 2x File Server, 1x NAS
    3x Windows Apps, 2x Linux Apps, 4x Web Server
    2x PostgreSQL, 1x MS SQL, 1x Oracle, 1x MongoDB
    3x VMware ESXi, 1x Kubernetes, 2x Docker

  Cloud-Backups:
    AWS S3, Azure Blob, Google Cloud

================================================================================
PARAMETER IM DETAIL
================================================================================

--size {small,medium,large}
    Unternehmensgröße. Bestimmt Anzahl Server und Standard-Tage.
    Default: medium

--db-type {postgresql,mysql}
    Datenbanktyp. Kann auch via DB_TYPE Umgebungsvariable gesetzt werden.

--host HOST
    Datenbank-Host. Default: localhost
    Umgebungsvariable: DB_HOST oder PGHOST

--port PORT
    Datenbank-Port. Default: 5432 (PostgreSQL) oder 3306 (MySQL)
    Umgebungsvariable: DB_PORT oder PGPORT

--database NAME
    Datenbankname. Default: bareos
    Umgebungsvariable: DB_NAME oder PGDATABASE

--user USER
    Datenbank-Benutzer.
    Umgebungsvariable: DB_USER oder PGUSER

--password PASS
    Datenbank-Passwort.
    Umgebungsvariable: DB_PASSWORD oder PGPASSWORD

--days TAGE
    Anzahl Tage Backup-Historie.
    Default: 30 (small), 60 (medium), 90 (large)

--success-rate PROZENT
    Prozentsatz erfolgreicher Jobs (0-100).
    Default: Realistisch pro Server-Typ (~90-95%)

    Beispiele:
      --success-rate 95   Fast alle Jobs erfolgreich
      --success-rate 70   Viele Fehler (zum Testen)
      --success-rate 50   Extremer Stress-Test

    Verteilung bei --success-rate X:
      - Erfolg (T): X%
      - Warning (W): (100-X) * 40%
      - Fehler (f/E/A): (100-X) * 60%

--no-logs
    Keine Job-Log-Einträge generieren.
    Deutlich schnellere Generierung, aber keine Log-Daten zum Testen.

--always-incremental / -a
    Verwendet die "Always Incremental" Backup-Strategie:
    - Ein initiales Full-Backup zu Beginn
    - Alle weiteren Backups sind Incremental
    - Periodische Consolidate-Jobs erstellen Virtual Fulls

    Vorteile dieser Strategie:
    - Schnellere Backups (nur Änderungen)
    - Weniger Storage-Verbrauch
    - Kürzere Backup-Fenster

--consolidate-interval TAGE / -i
    Abstand zwischen Consolidate-Jobs in Tagen.
    Default: 7 (wöchentlich)

    Beispiele:
      -i 7    Wöchentliche Consolidation
      -i 14   Zweiwöchentliche Consolidation
      -i 30   Monatliche Consolidation

--cleanup
    Alle generierten Testdaten aus der Datenbank löschen.
    Identifiziert Testdaten anhand des Job-Kommentars.

================================================================================
ALWAYS INCREMENTAL STRATEGIE
================================================================================

Bei aktivierter Always Incremental Strategie (-a / --always-incremental):

1. INITIALES FULL-BACKUP
   - Einmalig zu Beginn des Backup-Zeitraums
   - Vollständige Sicherung aller Daten

2. INKREMENTELLE BACKUPS
   - Alle nachfolgenden Backups sind inkrementell
   - Nur geänderte Daten seit dem letzten Backup
   - Intervall abhängig vom Server-Typ:
     * Datenbanken: alle 6 Stunden
     * Mail-Server: alle 12 Stunden
     * Andere: täglich

3. CONSOLIDATE JOBS
   - Führen mehrere Incrementals zu einem Virtual Full zusammen
   - Verkürzen die Backup-Kette für schnellere Restores
   - Job-Typ: 'c' (Consolidate)
   - Level: 'V' (Virtual Full)

Beispiel-Timeline (30 Tage, Consolidate-Intervall 7 Tage):
  Tag 1:   Full Backup
  Tag 2-7: Incremental Backups
  Tag 7:   Consolidate → Virtual Full
  Tag 8-14: Incremental Backups
  Tag 14:  Consolidate → Virtual Full
  ...

================================================================================
JOB-STATUS-CODES
================================================================================

  T  - Terminated normally (Erfolgreich)
  W  - Terminated with warnings (Mit Warnungen)
  f  - Failed (Fehlgeschlagen)
  E  - Terminated in Error (Fehler)
  A  - Canceled by user (Abgebrochen)

================================================================================
JOB-TYPEN
================================================================================

  B  - Backup (Standard-Backup-Job)
  c  - Consolidate (Virtual Full aus Incrementals)

================================================================================
BACKUP-LEVEL
================================================================================

  F  - Full (Vollbackup)
  I  - Incremental (Inkrementell)
  D  - Differential (Differenziell)
  V  - Virtual Full (Consolidate-Ergebnis)

================================================================================
GENERIERTE DATENMENGEN (GESCHÄTZT)
================================================================================

  Größe    Server   Jobs (30d)   Jobs (90d)   Log-Einträge
  -------  -------  -----------  -----------  -------------
  small    5        ~150         ~450         ~3.000
  medium   12       ~500         ~1.500       ~12.000
  large    ~100     ~5.000       ~15.000      ~150.000

================================================================================
JOB-LOG-FORMAT
================================================================================

Für jeden Job werden realistische Log-Einträge generiert:

  Start:
    JobId 123: Start Backup JobId 123, Job=Backup-dc01-fd
    JobId 123: Using Device "FileStorage" to write.

  Fortschritt:
    JobId 123: Files examined: 45000
    JobId 123: Files backed up: 42500

  Bei Fehlern:
    JobId 123: Cannot open file "/var/log/secure": Permission denied
    JobId 123: File "/tmp/cache.db" changed during backup.

  Ende:
    JobId 123: Backup OK. Files=42500 Bytes=1073741824 (1.00 GB)
    JobId 123: Backup completed successfully.

================================================================================
DATEIGRÖSZEN PRO SERVER-TYP
================================================================================

  Server-Typ            Dateien (Full)      Größe (Full)
  --------------------  ------------------  ------------------
  Domain Controller     30.000 - 100.000    15 - 50 GB
  File Server           200.000 - 2.000.000 200 GB - 2 TB
  Datenbank             500 - 20.000        50 GB - 1 TB
  Web Server            5.000 - 50.000      2 - 30 GB
  NAS                   500.000 - 5.000.000 500 GB - 10 TB
  VMware Host           50 - 500            200 GB - 2 TB

================================================================================
BEISPIELE
================================================================================

# Einfache Verwendung
python generate_bareos_testdata.py --size medium \\
    --db-type postgresql --user bareos --password geheim

# Mit Umgebungsvariablen
export DB_TYPE=postgresql DB_USER=bareos DB_PASSWORD=geheim
python generate_bareos_testdata.py --size large

# Hohe Erfolgsrate (95%)
python generate_bareos_testdata.py --size medium --success-rate 95

# Viele Fehler zum Testen (70% Erfolg)
python generate_bareos_testdata.py --size small --success-rate 70

# Enterprise mit 180 Tagen Historie
python generate_bareos_testdata.py --size large --days 180

# Schnelle Generierung ohne Logs
python generate_bareos_testdata.py --size large --no-logs

# Always Incremental Strategie (wöchentliche Consolidation)
python generate_bareos_testdata.py --size medium --always-incremental

# Always Incremental mit 14-tägiger Consolidation
python generate_bareos_testdata.py --size large -a -i 14

# Testdaten löschen
python generate_bareos_testdata.py --cleanup

================================================================================
ERSTELLTE DATENBANK-OBJEKTE
================================================================================

Pools:
  TestData-Full        - Für Full Backups
  TestData-Incremental - Für Incremental Backups
  TestData-Consolidate - Für Consolidate Jobs (nur bei -a)

FileSets:
  Windows-Standard     - Windows Server
  Linux-Standard       - Linux Server
  Database-Full        - Datenbanken
  NAS-Data             - NAS-Speicher
  VM-Image             - Virtuelle Maschinen
  Cloud-Sync           - Cloud-Backups

Clients:
  Naming: <servername>-fd
  Beispiele: dc01-fd, fs01-fd, web01-fd

Jobs:
  Kommentar: "Test data - generate_bareos_testdata.py"
  (Wird für Cleanup verwendet)

================================================================================
FEHLERBEHEBUNG
================================================================================

Verbindungsfehler:
  → Prüfen Sie Host, Port und ob der DB-Server läuft

Authentifizierungsfehler:
  → Prüfen Sie Benutzername und Passwort

Fehlende Tabellen:
  → Bareos-Datenbank muss initialisiert sein

Langsame Generierung:
  → --no-logs verwenden
  → --days reduzieren
  → Mit --size small testen

================================================================================
"""


# =============================================================================
# Main Entry Point
# =============================================================================

def print_short_help():
    """Print short help overview."""
    print(HELP_SHORT)
    sys.exit(0)


def print_detailed_help():
    """Print detailed help documentation."""
    print(HELP_DETAILED)
    sys.exit(0)


def main():
    # Check for double --help (--help --help or -h -h) before argparse
    help_count = sys.argv.count('--help') + sys.argv.count('-h')
    if help_count >= 2:
        print_detailed_help()

    # Environment variables
    env_db_type = os.environ.get('DB_TYPE', os.environ.get('db_type', ''))
    env_host = os.environ.get('DB_HOST', os.environ.get('db_host',
                os.environ.get('PGHOST', 'localhost')))
    env_port = os.environ.get('DB_PORT', os.environ.get('db_port',
                os.environ.get('PGPORT', '')))
    env_database = os.environ.get('DB_NAME', os.environ.get('db_name',
                os.environ.get('PGDATABASE', 'bareos')))
    env_user = os.environ.get('DB_USER', os.environ.get('db_user',
                os.environ.get('PGUSER', '')))
    env_password = os.environ.get('DB_PASSWORD', os.environ.get('db_password',
                os.environ.get('PGPASSWORD', '')))

    if not env_db_type and (os.environ.get('PGHOST') or os.environ.get('PGUSER')):
        env_db_type = 'postgresql'

    parser = argparse.ArgumentParser(
        description='Bareos Testdaten-Generator',
        formatter_class=argparse.RawDescriptionHelpFormatter,
        epilog="""
Unternehmensgrößen:
  small   - Kleine Firma (~6 Server, 30 Tage)
  medium  - Mittelstand (~12 Server, 60 Tage)
  large   - Enterprise (~50-200 Server, 90 Tage)

Kurzbeispiele:
  python generate_bareos_testdata.py --size medium --success-rate 95
  python generate_bareos_testdata.py --size large --days 180
  python generate_bareos_testdata.py --cleanup

Ausführliche Hilfe:
  python generate_bareos_testdata.py --help --help
  python generate_bareos_testdata.py --detailed-help
        """
    )

    # Main options
    parser.add_argument('-s', '--size', choices=['small', 'medium', 'large'],
                        default='medium',
                        help='Unternehmensgröße (default: medium)')

    # Database connection
    parser.add_argument('-t', '--db-type', default=env_db_type or None,
                        choices=['postgresql', 'mysql'],
                        help='Datenbanktyp (env: DB_TYPE)')
    parser.add_argument('-H', '--host', default=env_host,
                        help=f'Datenbank-Host (env: DB_HOST) [default: {env_host}]')
    parser.add_argument('-P', '--port', type=int,
                        default=int(env_port) if env_port else None,
                        help='Datenbank-Port (env: DB_PORT)')
    parser.add_argument('-D', '--database', default=env_database,
                        help=f'Datenbankname (env: DB_NAME) [default: {env_database}]')
    parser.add_argument('-u', '--user', default=env_user or None,
                        help='Datenbank-Benutzer (env: DB_USER)')
    parser.add_argument('-p', '--password', default=env_password or None,
                        help='Datenbank-Passwort (env: DB_PASSWORD)')

    # Generation options
    parser.add_argument('-d', '--days', type=int, default=None,
                        help='Tage Backup-Historie (default: abhängig von size)')
    parser.add_argument('-r', '--success-rate', type=int, default=None, metavar='%',
                        help='Erfolgsrate in Prozent (0-100)')
    parser.add_argument('-n', '--no-logs', action='store_true',
                        help='Ohne Job-Logs generieren (schneller)')
    parser.add_argument('-f', '--generate-files', action='store_true',
                        help='File-Einträge für jeden Job generieren (langsamer)')
    parser.add_argument('-a', '--always-incremental', action='store_true',
                        help='Always Incremental Strategie mit Consolidate Jobs')
    parser.add_argument('-i', '--consolidate-interval', type=int, default=7, metavar='TAGE',
                        help='Tage zwischen Consolidate Jobs (default: 7)')
    parser.add_argument('-c', '--cleanup', action='store_true',
                        help='Alle Testdaten löschen')
    parser.add_argument('-g', '--generate-configs', metavar='DIR',
                        help='Bareos-Konfigurationsdateien generieren in DIR')
    parser.add_argument('--configs-only', action='store_true',
                        help='Nur Konfigurationsdateien generieren (keine DB)')
    parser.add_argument('--detailed-help', action='store_true',
                        help='Ausführliche Hilfe anzeigen')

    args = parser.parse_args()

    # Check for detailed help (from argparse)
    if args.detailed_help:
        print_detailed_help()

    # Validate required arguments (only if not configs-only mode)
    if not args.configs_only:
        missing = []
        if not args.db_type:
            missing.append('-t/--db-type oder DB_TYPE')
        if not args.user:
            missing.append('-u/--user oder DB_USER')
        if not args.password:
            missing.append('-p/--password oder DB_PASSWORD')

        if missing:
            parser.error(f"Fehlende Parameter: {', '.join(missing)}")

    if args.port is None:
        args.port = 5432 if args.db_type == 'postgresql' else 3306

    # Determine days based on size if not specified
    if args.days is None:
        if args.size == 'small':
            args.days = SMALL_COMPANY['days_default']
        elif args.size == 'medium':
            args.days = MEDIUM_COMPANY['days_default']
        else:
            args.days = LARGE_COMPANY['days_default']

    try:
        # Build server list for config generation
        def get_servers_for_size(size: str) -> List[ServerConfig]:
            if size == 'small':
                return SMALL_COMPANY['servers']
            elif size == 'medium':
                return MEDIUM_COMPANY['servers']
            else:
                # Large company - build from templates
                servers = []
                for location in LARGE_COMPANY['locations']:
                    for server_type, prefix, count in LARGE_COMPANY['server_templates']:
                        for i in range(count):
                            name = f"{prefix}-{location.code.lower()}-{i+1:02d}"
                            desc = f"{server_type.value} at {location.name}"
                            servers.append(ServerConfig(name, server_type, desc))
                servers.extend(LARGE_COMPANY['cloud_servers'])
                return servers

        # Config-only mode (no database connection required)
        if args.configs_only:
            if not args.generate_configs:
                parser.error("--configs-only erfordert -g/--generate-configs DIR")

            # Create a dummy generator for config file generation only
            class ConfigOnlyGenerator:
                def generate_config_files(self, servers, output_dir, always_incremental, consolidate_interval):
                    import os as os_module

                    dirs = ['client', 'job', 'fileset', 'pool', 'schedule', 'storage', 'console']
                    for d in dirs:
                        path = os_module.path.join(output_dir, d)
                        os_module.makedirs(path, exist_ok=True)

                    print(f"\nGenerating configuration files in: {output_dir}")
                    print("-" * 60)

                    # Generate pools
                    pools = [
                        ('TestData-Full', 365, '50G', 100, 'Full'),
                        ('TestData-Incremental', 180, '10G', 500, 'Incr'),
                    ]
                    if always_incremental:
                        pools.append(('TestData-Consolidate', 365, '100G', 50, 'Consolidate'))

                    for pool_name, retention, max_bytes, max_vols, label_prefix in pools:
                        pool_content = CONFIG_TEMPLATES['pool'].format(
                            name=pool_name,
                            retention=retention,
                            max_bytes=max_bytes,
                            max_vols=max_vols,
                            label_prefix=label_prefix
                        )
                        pool_file = os_module.path.join(output_dir, 'pool', f'{pool_name}.conf')
                        with open(pool_file, 'w') as f:
                            f.write(pool_content)
                    print(f"  Created {len(pools)} pool configurations")

                    # Generate filesets
                    filesets = [
                        ('Linux-Standard', 'fileset_linux'),
                        ('Windows-Standard', 'fileset_windows'),
                        ('Database-Full', 'fileset_database'),
                    ]
                    for fs_name, template in filesets:
                        fs_content = CONFIG_TEMPLATES[template].format(name=fs_name)
                        fs_file = os_module.path.join(output_dir, 'fileset', f'{fs_name}.conf')
                        with open(fs_file, 'w') as f:
                            f.write(fs_content)
                    print(f"  Created {len(filesets)} fileset configurations")

                    # Generate schedules
                    schedules = []
                    if always_incremental:
                        schedules.append(('AlwaysIncremental-Weekly', 'schedule_always_incremental'))
                    else:
                        schedules.append(('FullWeekly-IncrDaily', 'schedule_full_weekly'))
                        schedules.append(('FullDaily-IncrHourly', 'schedule_full_daily'))

                    for sched_name, template in schedules:
                        if template == 'schedule_always_incremental':
                            sched_content = CONFIG_TEMPLATES[template].format(
                                name=sched_name,
                                full_pool='TestData-Full',
                                incr_pool='TestData-Incremental',
                                client_base='template',
                                fileset='Linux-Standard',
                                consolidate_schedule=f'{sched_name}-Consolidate',
                                consolidate_pool='TestData-Consolidate',
                                consolidate_interval=consolidate_interval
                            )
                        else:
                            sched_content = CONFIG_TEMPLATES[template].format(
                                name=sched_name,
                                full_pool='TestData-Full',
                                incr_pool='TestData-Incremental'
                            )
                        sched_file = os_module.path.join(output_dir, 'schedule', f'{sched_name}.conf')
                        with open(sched_file, 'w') as f:
                            f.write(sched_content)
                    print(f"  Created {len(schedules)} schedule configurations")

                    # Generate storage
                    storage_content = CONFIG_TEMPLATES['storage'].format(
                        name='FileStorage',
                        address='localhost',
                        password='TestStoragePassword123'
                    )
                    storage_file = os_module.path.join(output_dir, 'storage', 'FileStorage.conf')
                    with open(storage_file, 'w') as f:
                        f.write(storage_content)
                    print("  Created storage configuration")

                    # Generate console for testing
                    console_configs = [
                        ('onesimus', 'OneSimusPassword123', 'yes', 'yes'),
                        ('test-psk', 'TestPSKPassword123', 'yes', 'yes'),
                        ('test-legacy', 'TestLegacyPassword123', 'no', 'no'),
                    ]
                    for console_name, password, tls_enable, tls_require in console_configs:
                        console_content = CONFIG_TEMPLATES['console'].format(
                            name=console_name,
                            password=password,
                            tls_enable=tls_enable,
                            tls_require=tls_require
                        )
                        console_file = os_module.path.join(output_dir, 'console', f'{console_name}.conf')
                        with open(console_file, 'w') as f:
                            f.write(console_content)
                    print(f"  Created {len(console_configs)} console configurations")

                    # Generate clients and jobs
                    client_count = 0
                    job_count = 0

                    for server in servers:
                        password = f"TestPassword{random.randint(1000, 9999)}"
                        address = f"192.168.{random.randint(1, 254)}.{random.randint(1, 254)}"

                        if server.server_type in [ServerType.LINUX_DB, ServerType.WINDOWS_DB,
                                                   ServerType.POSTGRES_DB, ServerType.MSSQL_DB,
                                                   ServerType.ORACLE_DB, ServerType.MONGODB]:
                            fileset = 'Database-Full'
                            file_retention = 60
                            job_retention = 180
                        elif "LINUX" in server.server_type.name:
                            fileset = 'Linux-Standard'
                            file_retention = 30
                            job_retention = 90
                        else:
                            fileset = 'Windows-Standard'
                            file_retention = 30
                            job_retention = 90

                        client_content = CONFIG_TEMPLATES['client'].format(
                            name=server.name,
                            address=address,
                            password=password,
                            file_retention=file_retention,
                            job_retention=job_retention,
                            description=server.description
                        )
                        client_file = os_module.path.join(output_dir, 'client', f'{server.name}-fd.conf')
                        with open(client_file, 'w') as f:
                            f.write(client_content)
                        client_count += 1

                        if server.server_type in [ServerType.LINUX_DB, ServerType.WINDOWS_DB,
                                                   ServerType.POSTGRES_DB, ServerType.MSSQL_DB]:
                            if always_incremental:
                                schedule = 'AlwaysIncremental-Weekly'
                            else:
                                schedule = 'FullDaily-IncrHourly'
                            pool = 'TestData-Full'
                        else:
                            if always_incremental:
                                schedule = 'AlwaysIncremental-Weekly'
                            else:
                                schedule = 'FullWeekly-IncrDaily'
                            pool = 'TestData-Incremental'

                        job_content = CONFIG_TEMPLATES['job'].format(
                            client_name=server.name,
                            level='Full',
                            fileset=fileset,
                            schedule=schedule,
                            pool=pool,
                            description=server.description
                        )
                        job_file = os_module.path.join(output_dir, 'job', f'Backup-{server.name}.conf')
                        with open(job_file, 'w') as f:
                            f.write(job_content)
                        job_count += 1

                    print(f"  Created {client_count} client configurations")
                    print(f"  Created {job_count} job configurations")
                    print("-" * 60)
                    print(f"Configuration files generated in: {output_dir}")

            generator = ConfigOnlyGenerator()
            servers = get_servers_for_size(args.size)
            generator.generate_config_files(
                servers, args.generate_configs,
                args.always_incremental, args.consolidate_interval
            )
        else:
            # Normal mode with database connection
            generator = BareosTestDataGenerator(
                db_type=args.db_type,
                host=args.host,
                port=args.port,
                database=args.database,
                user=args.user,
                password=args.password
            )

            if args.cleanup:
                generator.cleanup_test_data()
            else:
                generate_logs = not args.no_logs

                # Convert percentage to ratio (0.0-1.0)
                success_rate = args.success_rate / 100.0 if args.success_rate is not None else None

                if args.size == 'small':
                    generator.generate_small_company(
                        args.days, generate_logs, args.generate_files, success_rate,
                        args.always_incremental, args.consolidate_interval
                    )
                elif args.size == 'medium':
                    generator.generate_medium_company(
                        args.days, generate_logs, args.generate_files, success_rate,
                        args.always_incremental, args.consolidate_interval
                    )
                else:
                    generator.generate_large_company(
                        args.days, generate_logs, args.generate_files, success_rate,
                        args.always_incremental, args.consolidate_interval
                    )

                # Also generate config files if requested
                if args.generate_configs:
                    servers = get_servers_for_size(args.size)
                    generator.generate_config_files(
                        servers, args.generate_configs,
                        args.always_incremental, args.consolidate_interval
                    )

            generator.close()

    except Exception as e:
        print(f"ERROR: {e}", file=sys.stderr)
        import traceback
        traceback.print_exc()
        sys.exit(1)


if __name__ == '__main__':
    main()
