# Onesimus - Prioritized Development Roadmap

## Priority 1: Foundation (CRITICAL - Do First)
*Get the database and core architecture working*

### 1.1 Database Schema ✓ (STARTED)
- [x] Create SQL schema generation scripts in Qt resources
- [x] Create SQL migration scripts in Qt resources
- [x] Add resources to Qt .qrc file
- [x] Design SQLite database schema with versioning support
- [x] Design unified settings table with history
- [x] Define tables for Directors, Clients, Storages
- [x] Implement version tracking table
- [x] Document database schema

### 1.2 Password Security (BLOCKER)
- [x] Research Bareos password hashing (MD5 vs alternatives)
- [x] Check Bareos/Bacula documentation for hash methods
- [x] Implement secure password hashing
- [x] Add password validation functions

### 1.3 Migration System
- [ ] Implement database migration framework
- [ ] Add version detection and automatic migration
- [ ] Connect to statusbar messages
- [ ] Load and execute SQL scripts from Qt resources
- [ ] Test migration from v1 to v2+ schemas

### 1.4 Release Preparation
- [ ] **BEFORE RELEASE**: Reset database schema to v1 (consolidate v1-v4 into single v1_initial.sql)
- [ ] Remove development migrations (v2, v3, v4) before first release
- [ ] Ensure LATEST_VERSION = 1 in bdatabase.h for initial release
- [ ] Document: Future releases will use v2, v3, etc. for migrations

---

## Priority 2: Director Management (CORE FEATURE)
*Get Director connection and management working*

### 2.1 Director Import
- [ ] Implement Director config tree import
- [ ] Parse Director configuration files
- [ ] Validate imported configuration
- [ ] Store Director passwords as hash in database

### 2.2 Multi-Director Support
- [ ] Support multiple Directors in database
- [ ] Allow switching between Directors
- [ ] Implement Director selection UI
- [ ] Handle concurrent connections

### 2.3 Director Export
- [ ] Implement Director export to ZIP
- [ ] Include all settings and metadata
- [ ] Implement import from ZIP

---

## Priority 3: Client Management (ESSENTIAL)
*Add client/FD configuration*

### 3.1 Client Import
- [ ] Implement Client and FD config import
- [ ] Parse Client resource definitions
- [ ] Link Clients to Directors

### 3.2 Client Wizard (Quick Win)
- [ ] Create fast Client wizard
- [ ] Implement quick setup with defaults
- [ ] Add validation

### 3.3 Client Dialog (Detailed)
- [ ] Create detailed Client dialog
- [ ] All available options
- [ ] Tooltips and help

### 3.4 Client Export
- [ ] Export to ZIP with FD settings
- [ ] Import from ZIP

---

## Priority 4: Storage Management (ESSENTIAL)
*Add storage/SD configuration*

### 4.1 Storage Import
- [ ] Implement Storage and SD config import
- [ ] Parse Storage resource definitions
- [ ] Link Storages to Directors

### 4.2 Storage Wizard (Quick Win)
- [ ] Create fast Storage wizard
- [ ] Quick setup for common types
- [ ] Add validation

### 4.3 Storage Dialog (Detailed)
- [ ] Create detailed Storage dialog
- [ ] Autochanger, devices, etc.
- [ ] Tooltips and help

### 4.4 Storage Export
- [ ] Export to ZIP with SD settings
- [ ] Import from ZIP

---

## Priority 5: Wizard Refactoring (INTEGRATION)
*Update existing wizards to use new database*

### 5.1 Update Import Wizard
- [ ] Refactor wizard to use new Director approach
- [ ] Update UI for new database structure
- [ ] Migrate wizard logic to new architecture
- [ ] Test with new database backend

---

## Priority 6: UI Enhancements (NICE TO HAVE)
*Polish and additional features*

### 6.1 Backup System Selection
- [ ] Add UI for Bareos/Bacula selection (BOTH mode)
- [ ] Settings dialog with dropdown
- [ ] Runtime switching between systems
- [ ] Save selection in QSettings

---

## Suggested Implementation Order

**Sprint 1: Database Foundation (Week 1)**
1. ✓ SQL resources setup
2. Database schema implementation
3. Password hashing research
4. Migration system

**Sprint 2: Director Core (Week 2)**
5. Director import
6. Multi-Director support
7. Basic Director UI

**Sprint 3: Client Management (Week 3)**
8. Client import
9. Client wizard (fast)
10. Client dialog (detailed)

**Sprint 4: Storage Management (Week 4)**
11. Storage import
12. Storage wizard (fast)
13. Storage dialog (detailed)

**Sprint 5: Integration & Polish (Week 5)**
14. Wizard refactoring
15. Export/Import functionality
16. System selection UI
17. Testing and bug fixes

---

## Quick Wins (Do These Early for Morale)
1. ✓ SQL resources in Qt (.qrc) - DONE
2. Database schema creation from resources
3. Simple Director wizard working
4. First successful Director connection test

---

## Blockers to Watch For
- **Password hashing compatibility** - Must match Bareos/Bacula exactly
- **Config file parsing** - Bareos/Bacula config syntax can be complex
- **TLS/SSL handling** - Especially on Windows with Schannel
- **Database migrations** - Must be bulletproof to avoid data loss

---

*Last Updated: 2026-01-31*
*Priority: Focus on getting Director management working first, then expand to Client/Storage*
