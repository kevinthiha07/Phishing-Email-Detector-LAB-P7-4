import os
import email

SUSPICIOUS_KEYWORDS = [
    "urgent", "verify", "account", "password", "click", "login",
    "bank", "update", "confirm", "limited", "immediately", "winner"
]

def keyword_position_scoring(subject: str, body: str) -> int:
    """
    Assign a phishing risk score based on suspicious keyword positions.
    - Subject keywords: +5 points
    - Keywords in first 200 characters of body: +3 points
    - Keywords in rest of body: +1 point
    """
    score = 0
    subject_lower = subject.lower()
    body_lower = body.lower()

    for keyword in SUSPICIOUS_KEYWORDS:
        if keyword in subject_lower:
            score += 5

    early_body = body_lower[:200]
    for keyword in SUSPICIOUS_KEYWORDS:
        if keyword in early_body:
            score += 3

    for keyword in SUSPICIOUS_KEYWORDS:
        if keyword in body_lower[200:]:
            score += 1

    return score


def extract_subject_body(filepath: str):
    """
    Extract subject and body from a raw email file.
    Handles plain text and HTML formats.
    """
    with open(filepath, "r", encoding="latin1") as f:
        raw_email = f.read()

    msg = email.message_from_string(raw_email)

    subject = msg.get("Subject", "")

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
            score = keyword_position_scoring(subject, body)
            print(f"{filename} | Subject: {subject[:60]} | Score: {score}")
