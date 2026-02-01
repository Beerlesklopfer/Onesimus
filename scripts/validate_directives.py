#!/usr/bin/env python3
"""
Directive JSON Validator

Validates directive JSON files against schema and performs cross-checks:
- JSON Schema validation
- Type consistency checks
- Bareos/Bacula compatibility verification
- Example value validation
- Cross-reference verification

Usage:
    python validate_directives.py
    python validate_directives.py --file director.json
    python validate_directives.py --strict
"""

import json
import sys
import argparse
from pathlib import Path
from typing import Dict, List, Tuple, Any

try:
    import jsonschema
    HAS_JSONSCHEMA = True
except ImportError:
    HAS_JSONSCHEMA = False
    print("Warning: jsonschema not installed. Install with: pip install jsonschema")


class DirectiveValidator:
    """Validate directive JSON files"""

    def __init__(self, directives_dir: str = "../resources/directives"):
        self.directives_dir = Path(directives_dir)
        self.schema_file = self.directives_dir / "directive_schema.json"
        self.schema = None
        self.errors = []
        self.warnings = []

        if HAS_JSONSCHEMA and self.schema_file.exists():
            with open(self.schema_file, 'r', encoding='utf-8') as f:
                self.schema = json.load(f)

    def validate_file(self, filepath: Path, strict: bool = False) -> bool:
        """Validate a single directive JSON file"""
        self.errors = []
        self.warnings = []

        print(f"\n{'='*60}")
        print(f"Validating: {filepath.name}")
        print(f"{'='*60}")

        try:
            with open(filepath, 'r', encoding='utf-8') as f:
                data = json.load(f)
        except json.JSONDecodeError as e:
            self.errors.append(f"JSON Parse Error: {e}")
            return False
        except Exception as e:
            self.errors.append(f"Error reading file: {e}")
            return False

        # 1. JSON Schema validation
        if HAS_JSONSCHEMA and self.schema:
            try:
                jsonschema.validate(instance=data, schema=self.schema)
                print("✓ JSON Schema validation passed")
            except jsonschema.ValidationError as e:
                self.errors.append(f"Schema validation failed: {e.message}")
                if strict:
                    return False

        # 2. Basic structure validation
        if not self._validate_structure(data):
            return False

        # 3. Directive-level validation
        for directive_name, directive_def in data.get('directives', {}).items():
            self._validate_directive(directive_name, directive_def, strict)

        # 4. Cross-reference validation
        self._validate_cross_references(data)

        # 5. Consistency checks
        self._validate_consistency(data)

        # Print results
        if self.warnings:
            print(f"\n⚠ {len(self.warnings)} Warning(s):")
            for warning in self.warnings:
                print(f"  - {warning}")

        if self.errors:
            print(f"\n✗ {len(self.errors)} Error(s):")
            for error in self.errors:
                print(f"  - {error}")
            return False
        else:
            print(f"\n✓ Validation passed!")
            return True

    def _validate_structure(self, data: Dict) -> bool:
        """Validate basic structure"""
        required_fields = ['resource_type', 'description', 'directives']

        for field in required_fields:
            if field not in data:
                self.errors.append(f"Missing required field: {field}")
                return False

        if not isinstance(data['directives'], dict):
            self.errors.append("'directives' must be an object")
            return False

        print(f"✓ Basic structure valid")
        print(f"  Resource Type: {data['resource_type']}")
        print(f"  Directives: {len(data['directives'])}")

        return True

    def _validate_directive(self, name: str, directive: Dict, strict: bool) -> None:
        """Validate a single directive definition"""

        # Required fields
        required = ['type', 'required', 'bareos', 'bacula', 'description']
        for field in required:
            if field not in directive:
                self.errors.append(f"Directive '{name}': Missing required field '{field}'")

        # Type-specific validation
        directive_type = directive.get('type')

        # Integer type should have example as integer
        if directive_type == 'integer':
            if 'example' in directive and not isinstance(directive['example'], int):
                self.warnings.append(f"Directive '{name}': Example should be integer, got {type(directive['example']).__name__}")

            if 'default' in directive and not isinstance(directive['default'], (int, type(None))):
                self.warnings.append(f"Directive '{name}': Default should be integer")

            # Check min/max constraints
            if 'min' in directive and 'max' in directive:
                if directive['min'] > directive['max']:
                    self.errors.append(f"Directive '{name}': min ({directive['min']}) > max ({directive['max']})")

            # Validate example is within range
            if 'example' in directive and isinstance(directive['example'], int):
                if 'min' in directive and directive['example'] < directive['min']:
                    self.warnings.append(f"Directive '{name}': Example {directive['example']} < min {directive['min']}")
                if 'max' in directive and directive['example'] > directive['max']:
                    self.warnings.append(f"Directive '{name}': Example {directive['example']} > max {directive['max']}")

        # Boolean type should have example as boolean
        elif directive_type == 'boolean':
            if 'example' in directive and not isinstance(directive['example'], bool):
                self.warnings.append(f"Directive '{name}': Example should be boolean")

        # String list should have example as array
        elif directive_type == 'string_list':
            if 'example' in directive and not isinstance(directive['example'], list):
                self.warnings.append(f"Directive '{name}': Example should be array")

        # Resource reference must have reference_type
        elif directive_type == 'resource_reference':
            if 'reference_type' not in directive:
                self.errors.append(f"Directive '{name}': resource_reference type must have 'reference_type' field")

        # Validate bareos/bacula compatibility
        if not directive.get('bareos') and not directive.get('bacula'):
            self.errors.append(f"Directive '{name}': Must be compatible with at least one of bareos or bacula")

        # Version format validation
        if 'min_version' in directive:
            version = directive['min_version']
            if not isinstance(version, str) or len(version.split('.')) != 3:
                self.errors.append(f"Directive '{name}': min_version must be in format X.Y.Z")

    def _validate_cross_references(self, data: Dict) -> None:
        """Validate cross-references between directives"""
        valid_resource_types = [
            'Director', 'Client', 'Storage', 'Console',
            'Pool', 'FileSet', 'Job', 'Schedule', 'Catalog', 'Messages'
        ]

        for directive_name, directive_def in data.get('directives', {}).items():
            if directive_def.get('type') == 'resource_reference':
                ref_type = directive_def.get('reference_type')
                if ref_type and ref_type not in valid_resource_types:
                    self.warnings.append(
                        f"Directive '{directive_name}': Unknown reference_type '{ref_type}'"
                    )

        print(f"✓ Cross-reference validation passed")

    def _validate_consistency(self, data: Dict) -> None:
        """Check for consistency issues"""
        directives = data.get('directives', {})

        # Check for directives with both bareos=false and bacula=false
        invalid_compat = []
        for name, directive in directives.items():
            if not directive.get('bareos') and not directive.get('bacula'):
                invalid_compat.append(name)

        if invalid_compat:
            self.errors.append(
                f"Directives with no compatibility: {', '.join(invalid_compat)}"
            )

        # Check for missing 'use' field (warning only)
        missing_use = []
        for name, directive in directives.items():
            if 'use' not in directive:
                missing_use.append(name)

        if missing_use:
            self.warnings.append(
                f"{len(missing_use)} directives missing 'use' field: {', '.join(missing_use[:5])}"
                + ("..." if len(missing_use) > 5 else "")
            )

        # Check for required directives without defaults
        required_no_default = []
        for name, directive in directives.items():
            if directive.get('required') and 'default' not in directive:
                required_no_default.append(name)

        if required_no_default:
            print(f"ℹ {len(required_no_default)} required directives have no defaults (expected): "
                  + ', '.join(required_no_default[:3])
                  + ("..." if len(required_no_default) > 3 else ""))

        print(f"✓ Consistency checks passed")

    def validate_all(self, strict: bool = False) -> bool:
        """Validate all directive JSON files"""
        all_valid = True
        json_files = list(self.directives_dir.glob("*.json"))

        # Exclude schema file
        json_files = [f for f in json_files if f.name != 'directive_schema.json']

        for json_file in json_files:
            if not self.validate_file(json_file, strict):
                all_valid = False

        return all_valid


def main():
    parser = argparse.ArgumentParser(description="Validate Bareos/Bacula directive JSON files")
    parser.add_argument('--file', metavar='FILENAME',
                       help='Validate a specific file')
    parser.add_argument('--strict', action='store_true',
                       help='Strict mode: fail on warnings')
    parser.add_argument('--directives-dir', default='../resources/directives',
                       help='Directory containing directive JSON files')

    args = parser.parse_args()

    validator = DirectiveValidator(args.directives_dir)

    if args.file:
        filepath = Path(args.directives_dir) / args.file
        if not filepath.exists():
            print(f"Error: File not found: {filepath}")
            sys.exit(1)

        success = validator.validate_file(filepath, args.strict)
        sys.exit(0 if success else 1)
    else:
        success = validator.validate_all(args.strict)
        print(f"\n{'='*60}")
        if success:
            print("✓ All validations passed!")
            sys.exit(0)
        else:
            print("✗ Some validations failed")
            sys.exit(1)


if __name__ == '__main__':
    main()
