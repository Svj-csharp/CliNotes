# CliNotes
simple CLI util to create and edit your notes.
BAased on CLI11

# How to start
git clone https://github.com/Svj-csharp/CliNotes && cd CliNotes
chmoid +x install.sh
./install.sh

# How to use
**Create a new note folder and initialize its content.**
clinote -t "MyNote" create_note	
**Create a note in a specific directory (and log this path).**
clinote -u ~/custom/path -t "Work" create_note	
**Open interactive menu to append or overwrite the note content.**
clinote -t "MyNote" write_note	
**Display the content of the specified note in the terminal.**
clinote -t "MyNote" read_note
**List all notes from all previously used directories (via logs).**
clinote list_notes	
**Permanently delete the note folder and its content.**
clinote -t "MyNote" delete_note	
