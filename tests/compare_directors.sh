#!/bin/bash
# Wrapper for compare_directors.py
exec python3 "$(dirname "$0")/compare_directors.py" "$@"
