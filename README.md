# ProcessDB Manager

## Overview
ProcessDB Manager is a Qt-based desktop application that combines SQLite database management with external process control. It provides a user-friendly interface for managing database records and controlling external applications, making it ideal for system administrators and database managers.

## Features
- **Database Management**
  - SQLite database integration
  - Real-time data viewing and editing
  - Duplicate entry prevention
  - Data validation
  
- **Process Control**
  - External process launching
  - Process monitoring
  - Safe process termination
  
- **User Interface**
  - Modern Qt-based GUI
  - Table view for data visualization
  - Combo box for quick record selection
  - Resizable columns
  
- **Data Safety**
  - Input validation
  - Data integrity checks
  - Safe database operations

## Technical Specifications
- Built with Qt 5/6
- C++17 standard
- SQLite database backend
- Cross-platform compatibility
- Internationalization support

## Prerequisites
- Qt 5/6 development libraries
- SQLite 3
- C++17 compatible compiler
- CMake 3.5 or higher

## Project Structure
```
processdb-manager/
├── CMakeLists.txt          # Build configuration
├── main.cpp                # Application entry point
├── mainwindow.cpp          # Main window implementation
├── mainwindow.h           # Main window header
├── mainwindow.ui          # UI design file
└── untitled_en_GB.ts      # Translation file
```

## Building the Project

### 1. Configure Build Environment

#### Windows
```bash
# Install Qt and required tools
# Set up environment variables
```

#### Linux
```bash
# Ubuntu/Debian
sudo apt-get install qt6-base-dev
sudo apt-get install libsqlite3-dev
```

#### macOS
```bash
brew install qt6
brew install sqlite
```

### 2. Build Instructions
```bash
# Create build directory
mkdir build
cd build

# Configure CMake
cmake ..

# Build the project
cmake --build .
```

## Database Configuration
1. Create a SQLite database named `comp.db`
2. Update the database path in `mainwindow.cpp`:
   ```cpp
   db.setDatabaseName("path/to/your/comp.db");
   ```

## Usage

### Database Operations
1. **View Records**
   - Launch the application
   - Records are displayed in the table view
   - Click on any record to view details

2. **Add New Record**
   - Enter data in the text fields
   - Click "Add" button
   - System validates for duplicates

3. **Update Record**
   - Select existing record
   - Modify fields
   - Click "Add" button to update

### Process Management
1. **Start Process**
   - Configure executable path in settings
   - Click "Start" button
   - Monitor process status

2. **Stop Process**
   - Click "Stop" button for safe termination
   - System ensures clean process shutdown

## Development Guide

### Adding New Features
1. Update UI in Qt Designer
2. Implement functionality in `mainwindow.cpp`
3. Update header files as needed
4. Add new database operations in appropriate sections

### Database Schema
Table structure for `m` table:
- id (Primary Key)
- name (Unique)
- link

### Internationalization
- Translation files in `untitled_en_GB.ts`
- Use Qt Linguist for translations
- Support for additional languages

## Contributing
1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## Troubleshooting
- Database Connection: Verify path and permissions
- Process Control: Check executable paths
- UI Issues: Verify Qt version compatibility

## Acknowledgments
- Qt Framework
- SQLite
