import os
import email
import sys

SUSSY_WORDS = ["Urgent", "Verify", "Account", "Password", "Day"]


def count_suspicious_words(subject, body, SUSSY_WORDS):
    """
    Count occurrences of suspicious words in subject and body.
    Returns a dictionary {word: count}.
    """
    SusCount = {}
    # Convert to lower for case-insensitive matching
    subject_lower = subject.lower()
    body_lower = body.lower()
    for Kword in SUSSY_WORDS:
        count = subject_lower.count(Kword.lower()) + body_lower.count(Kword.lower())
        SusCount[Kword] = count
    return SusCount

def extract_subject_body(filepath: str):
    """
    Extract subject and body from a raw email file.
    Handles plain text and HTML formats.
    """
    with open(filepath, "r", encoding="latin1") as f:
        raw_email = f.read()

    msg = email.message_from_string(raw_email)

    # Extract subject
    subject = msg.get("Subject", "")

    # Extract body
    body = ""
    if msg.is_multipart():
        for part in msg.walk():
            content_type = part.get_content_type()
            if content_type == "text/plain":
                try:
                    body = part.get_payload(decode=True).decode(errors="ignore")
                except:
                    body = part.get_payload()
                break
            elif content_type == "text/html" and not body:  # fallback if no plain text
                try:
                    body = part.get_payload(decode=True).decode(errors="ignore")
                except:
                    body = part.get_payload()
    else:
        try:
            body = msg.get_payload(decode=True).decode(errors="ignore")
        except:
            body = msg.get_payload()

    return subject, body

if __name__ == "__main__":
    DATASET_FOLDER = "datasets"  # adjust this if needed

    for filename in os.listdir(DATASET_FOLDER):
        filepath = os.path.join(DATASET_FOLDER, filename)
        if os.path.isfile(filepath):
            subject, body = extract_subject_body(filepath)
            SussCounted = count_suspicious_words(subject, body, SUSSY_WORDS)
            summary = ", ".join(f"{k}:{v}" for k, v in SussCounted.items())
            print(f"{filename} | Subject: {subject[:60]} | Keyword Counter: {summary}")