#!/bin/sh

# Shell script for bash that uploads all files in a directory to git

# To make executable:
# Run this command in the terminal: chmod a+x (yourscriptname)
# Open with terminal on MacOSX

clear >$(tty) # Clear terminal text
cd "$(dirname "$0")" # Change to current directory
read -p "Git commit comment: "  comment # Prompt for name
git add . # Track all files
git add -u # Track deleted files
git commit -m "$comment" # Commit
git push origin master # Push to server