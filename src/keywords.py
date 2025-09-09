import os

SussyWord = ("Urgent", "Verify", "Account", "Password")

folder_path = r"C:\Users\go923\Documents\Phishing-Email-Detector-LAB-P7-4\datasets"
output_file = "combined.mbox"

files = [f for f in os.listdir(folder_path) if os.path.isfile(os.path.join(folder_path, f))]

with open(output_file, 'w', encoding='utf-8') as out_f:
    for filename in files:
        file_path = os.path.join(folder_path, filename)
        with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
            out_f.write(f.read())
            out_f.write('\n\n')  # Separate messages with blank lines

print(f"Combined {len(files)} messages into {output_file}")

# Set the path to your mbox file
mbox_path = "combined.mbox"
characters_to_check = ["Urgent", "Verify", "Account", "Password"]  # Add any characters you want to check

# Read the entire mbox file
with open(mbox_path, 'r', encoding='utf-8', errors='ignore') as f:
    mbox_content = f.read()

# Count occurrences
char_counts = {char: mbox_content.count(char) for char in characters_to_check}

# Print results
for char, count in char_counts.items():
    print(f"Character '{char}' found {count} times.")