#!/usr/bin/env python3
"""
FileSet Template Generator

This script helps create and manage FileSet templates for common backup scenarios.
It can generate template JSON files from predefined configurations.

Usage:
    python generate_fileset_templates.py --list
    python generate_fileset_templates.py --create windows_exchange
    python generate_fileset_templates.py --validate all
"""

import json
import os
import argparse
from pathlib import Path
from typing import Dict, List, Any

# Template definitions
TEMPLATE_DEFINITIONS = {
    "windows_exchange": {
        "name": "Microsoft Exchange Server",
        "description": "Backup of Exchange Server databases, logs, and configuration",
        "platform": "windows",
        "category": "database",
        "backup_system": "both",
        "fileset": {
            "Name": "WindowsExchangeServer",
            "Description": "Exchange mailbox databases and logs",
            "Enable VSS": True,
            "Ignore FileSet Changes": False,
            "Include": {
                "Options": {
                    "Signature": "MD5",
                    "Compression": "GZIP",
                    "Accurate": True,
                    "IgnoreCase": True,
                },
                "File": [
                    "C:/Program Files/Microsoft/Exchange Server",
                    "D:/ExchangeDB",
                    "E:/ExchangeLogs",
                    "C:/Windows/System32/config"
                ]
            },
            "Plugin": "exchange:"
        },
        "recommended_schedule": "Hourly Transaction Log, Daily Differential, Weekly Full",
        "estimated_size": "100 GB - 10 TB",
        "notes": [
            "Requires Exchange VSS Writer",
            "Use exchange plugin for application-aware backups",
            "Transaction log truncation after successful backup",
            "Test mailbox restoration regularly"
        ]
    },

    "linux_apache_nginx": {
        "name": "Linux Web Server (Apache/Nginx)",
        "description": "Backup of Apache or Nginx web server configuration and sites",
        "platform": "linux",
        "category": "web_server",
        "backup_system": "both",
        "fileset": {
            "Name": "LinuxWebServer",
            "Description": "Web server configuration and sites",
            "Enable VSS": False,
            "Ignore FileSet Changes": False,
            "Include": {
                "Options": {
                    "Signature": "MD5",
                    "Compression": "GZIP",
                    "OneFS": True,
                    "Accurate": True,
                    "Exclude": {
                        "WildFile": ["*.log", "*.tmp"],
                        "WildDir": ["*/logs", "*/tmp", "*/cache"]
                    }
                },
                "File": [
                    "/etc/apache2",
                    "/etc/nginx",
                    "/etc/httpd",
                    "/var/www",
                    "/usr/share/nginx/html",
                    "/etc/letsencrypt"
                ]
            }
        },
        "recommended_schedule": "Hourly Incremental, Daily Full",
        "estimated_size": "5-100 GB",
        "notes": [
            "Includes SSL certificates from /etc/letsencrypt",
            "Excludes log files by default",
            "Adjust paths based on distribution (Debian/Ubuntu vs RHEL/CentOS)",
            "Test site restoration including virtual host configs"
        ]
    }
}


class FileSetTemplateGenerator:
    """Generate and manage FileSet templates"""

    def __init__(self, output_dir: str = "../resources/templates/filesets"):
        self.output_dir = Path(output_dir)
        self.output_dir.mkdir(parents=True, exist_ok=True)

    def list_templates(self) -> List[str]:
        """List all available template definitions"""
        return list(TEMPLATE_DEFINITIONS.keys())

    def create_template(self, template_id: str) -> bool:
        """Create a template JSON file from definition"""
        if template_id not in TEMPLATE_DEFINITIONS:
            print(f"Error: Template '{template_id}' not found")
            return False

        template_data = TEMPLATE_DEFINITIONS[template_id]
        output_file = self.output_dir / f"{template_id}.json"

        try:
            with open(output_file, 'w', encoding='utf-8') as f:
                json.dump(template_data, f, indent=2, ensure_ascii=False)

            print(f"Created template: {output_file}")
            return True
        except Exception as e:
            print(f"Error creating template: {e}")
            return False

    def validate_template(self, template_file: Path) -> bool:
        """Validate a template JSON file"""
        try:
            with open(template_file, 'r', encoding='utf-8') as f:
                data = json.load(f)

            # Required fields
            required_fields = ['name', 'description', 'platform', 'category', 'backup_system', 'fileset']
            for field in required_fields:
                if field not in data:
                    print(f"Error in {template_file.name}: Missing required field '{field}'")
                    return False

            # Validate platform
            if data['platform'] not in ['windows', 'linux', 'both']:
                print(f"Error in {template_file.name}: Invalid platform '{data['platform']}'")
                return False

            # Validate backup_system
            if data['backup_system'] not in ['bareos', 'bacula', 'both']:
                print(f"Error in {template_file.name}: Invalid backup_system '{data['backup_system']}'")
                return False

            print(f"✓ {template_file.name} is valid")
            return True

        except json.JSONDecodeError as e:
            print(f"Error parsing {template_file.name}: {e}")
            return False
        except Exception as e:
            print(f"Error validating {template_file.name}: {e}")
            return False

    def validate_all(self) -> bool:
        """Validate all template files in output directory"""
        all_valid = True
        for template_file in self.output_dir.glob("*.json"):
            if not self.validate_template(template_file):
                all_valid = False
        return all_valid

    def create_all(self) -> None:
        """Create all defined templates"""
        for template_id in TEMPLATE_DEFINITIONS:
            self.create_template(template_id)

    def export_to_cpp(self, output_file: str = "../include/bfileset_templates.h") -> bool:
        """Export templates as C++ header file for embedding"""
        try:
            with open(output_file, 'w') as f:
                f.write("// Auto-generated FileSet templates\n")
                f.write("#ifndef BFILESET_TEMPLATES_H\n")
                f.write("#define BFILESET_TEMPLATES_H\n\n")
                f.write("#include <QString>\n")
                f.write("#include <QMap>\n\n")
                f.write("namespace BFileSetTemplates {\n\n")

                for template_id, template_data in TEMPLATE_DEFINITIONS.items():
                    json_str = json.dumps(template_data, indent=2)
                    f.write(f"const QString {template_id.upper()} = R\"(\n")
                    f.write(json_str)
                    f.write("\n)\";\n\n")

                f.write("} // namespace BFileSetTemplates\n\n")
                f.write("#endif // BFILESET_TEMPLATES_H\n")

            print(f"Exported templates to {output_file}")
            return True
        except Exception as e:
            print(f"Error exporting to C++: {e}")
            return False


def main():
    parser = argparse.ArgumentParser(description="FileSet Template Generator")
    parser.add_argument('--list', action='store_true',
                       help='List all available templates')
    parser.add_argument('--create', metavar='TEMPLATE_ID',
                       help='Create a specific template')
    parser.add_argument('--create-all', action='store_true',
                       help='Create all templates')
    parser.add_argument('--validate', metavar='TEMPLATE',
                       help='Validate a template (use "all" for all templates)')
    parser.add_argument('--export-cpp', action='store_true',
                       help='Export templates as C++ header file')
    parser.add_argument('--output-dir', default='../resources/templates/filesets',
                       help='Output directory for templates')

    args = parser.parse_args()

    generator = FileSetTemplateGenerator(args.output_dir)

    if args.list:
        print("Available templates:")
        for template_id in generator.list_templates():
            template = TEMPLATE_DEFINITIONS[template_id]
            print(f"  {template_id}: {template['name']} ({template['platform']})")

    elif args.create:
        generator.create_template(args.create)

    elif args.create_all:
        generator.create_all()

    elif args.validate:
        if args.validate == 'all':
            if generator.validate_all():
                print("\n✓ All templates are valid")
            else:
                print("\n✗ Some templates have errors")
        else:
            template_file = Path(args.output_dir) / f"{args.validate}.json"
            generator.validate_template(template_file)

    elif args.export_cpp:
        generator.export_to_cpp()

    else:
        parser.print_help()


if __name__ == '__main__':
    main()
