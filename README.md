# Text Editor in C++

This project is a C++ implementation of a console-based text editor, offering essential features for text manipulation, navigation, and file handling, inspired by command-line editors like Vim.

## Features

### Text Editing
- Insert, delete, and replace characters or lines.
- Support for creating new lines and merging adjacent lines.
- Move the cursor by character, word, or line.

### File Handling
- Load text files for editing.
- Save changes to files with optional filename specification.
- Check and handle file permissions.

### Search and Replace
- Find occurrences of a keyword in the text.
- Replace the first or all occurrences of a keyword.
- Navigation through search results.

### Command Mode
- Supports commands for various operations:
  - `dd`: Delete the current line.
  - `D`: Delete characters from the cursor to the end of the line.
  - `x`: Delete a character at the cursor.
  - `yy`: Copy the current line.
  - `p`, `P`: Paste copied lines after or before the current line.
  - `s/find/replace/`: Replace the first occurrence of "find" with "replace".
  - `s/find/replace/g`: Replace all occurrences of "find" with "replace".

### Visual Feedback
- Highlight search results and the current cursor position.
- Display line numbers, cursor position, and other metadata.

### Navigation
- Move cursor using arrow keys.
- Navigate between words or lines.
- Jump to the beginning or end of a line.

### Customization and Undo
- Add or remove indentation to lines.
- Yank multiple lines for reuse.
- Undo and redo changes within the current session.

### Interactive CLI
- Command-driven interface for precise text operations.
- Command history navigation for efficient editing.

## Installation and Usage

1. Clone the repository:
   ```bash
   git clone <repository-url>
   ```
2. Compile the project using a C++ compiler with Windows dependencies:
   ```bash
   g++ -o texteditor BSCS23034_Project_phase02.cpp
   ```
3. Run the editor:
   ```bash
   ./texteditor
   ```

## Commands

- **Insert Mode**: Press `i` to switch to insert mode.
- **View Mode**: Press `ESC` to exit insert mode and navigate.
- **Command Mode**: Press `:` to execute commands.
- **Search Mode**: Use `/` followed by a keyword to search in the text.
- **Save and Quit**:
  - `:w filename` - Save the file.
  - `:q` - Quit the editor.
  - `:wq` - Save and quit.

## File Structure

- **TextEditor Class**: Core logic for text editing and navigation.
- **SearchEngine Class**: Handles keyword-based search and replace.
- **FileManager Class**: Manages file reading and writing.
- **Doubly Linked List Template**: Underlying data structure for storing and manipulating text lines.

## Dependencies

- Windows API (for console operations like cursor movement).
- Standard C++ libraries.

## Limitations and Future Improvements

- **Portability**: Current implementation depends on Windows-specific APIs.
- **Undo/Redo**: Add a feature for multi-level undo and redo.
- **Plugins**: Extend functionality with plugins for advanced editing.
- **Syntax Highlighting**: Add support for syntax highlighting.

## License

This project is open-source and available under the [MIT License](LICENSE).

## Author

[Muhammad Zair]
