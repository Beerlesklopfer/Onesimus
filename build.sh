#!/bin/bash

# Bacula Qt UI Build-Script mit OpenSSL-Unterstützung
# Dieses Script automatisiert den Build-Prozess

set -e  # Beende bei Fehler

echo "================================"
echo "Bacula Qt UI - Build Script"
echo "================================"
echo ""

# Prüfe ob CMake installiert ist
if ! command -v cmake &> /dev/null; then
    echo "FEHLER: CMake ist nicht installiert!"
    echo "Installation: sudo apt-get install cmake"
    exit 1
fi

# Prüfe ob Qt6 verfügbar ist
if ! command -v qmake6 &> /dev/null && ! command -v qmake &> /dev/null; then
    echo "WARNUNG: Qt6 möglicherweise nicht installiert!"
    echo "Installation: sudo apt-get install qt6-base-dev"
fi

# Git-Submodule initialisieren
echo "Prüfe Git-Submodule..."
if [ ! -f "external/openssl/Configure" ]; then
    echo "OpenSSL Submodule nicht initialisiert. Initialisiere..."
    git submodule update --init --recursive
    echo "Submodule erfolgreich initialisiert."
else
    echo "OpenSSL Submodule bereits initialisiert."
fi
echo ""

# Build-Option wählen
echo "OpenSSL Build-Option:"
echo "1) Statisch (aus Submodule) - empfohlen für Deployment"
echo "2) System OpenSSL - schneller Build"
echo ""
read -p "Wählen Sie [1/2] (Standard: 1): " OPENSSL_OPTION
OPENSSL_OPTION=${OPENSSL_OPTION:-1}

if [ "$OPENSSL_OPTION" = "1" ]; then
    USE_STATIC_OPENSSL=ON
    echo "Verwende statisches OpenSSL (wird kompiliert)"
else
    USE_STATIC_OPENSSL=OFF
    echo "Verwende System-OpenSSL"
fi
echo ""

# Erstelle Build-Verzeichnis
if [ -d "build" ]; then
    echo "Build-Verzeichnis existiert bereits."
    read -p "Möchten Sie es neu erstellen? (j/n) " -n 1 -r
    echo ""
    if [[ $REPLY =~ ^[Jj]$ ]]; then
        echo "Lösche altes Build-Verzeichnis..."
        rm -rf build
    fi
fi

if [ ! -d "build" ]; then
    echo "Erstelle Build-Verzeichnis..."
    mkdir build
fi

cd build

echo ""
echo "================================"
echo "CMake-Konfiguration"
echo "================================"
echo ""

cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DUSE_STATIC_OPENSSL=$USE_STATIC_OPENSSL

echo ""
echo "================================"
echo "Kompilierung"
echo "================================"
echo ""

if [ "$USE_STATIC_OPENSSL" = "ON" ]; then
    echo "HINWEIS: OpenSSL wird kompiliert - dies kann 5-10 Minuten dauern..."
    echo ""
fi

make -j$(nproc)

cd ..

echo ""
echo "================================"
echo "Build erfolgreich abgeschlossen!"
echo "================================"
echo ""
echo "Ausführbare Datei: ./build/BaculaQtUI"
echo ""

if [ "$USE_STATIC_OPENSSL" = "ON" ]; then
    echo "✓ OpenSSL wurde statisch gelinkt"
    echo "✓ Keine externen OpenSSL-Bibliotheken erforderlich"
fi

echo ""
echo "Programm starten mit: ./build/BaculaQtUI"
echo "Oder installieren mit: cd build && sudo make install"
echo ""

