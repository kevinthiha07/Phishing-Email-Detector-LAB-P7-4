import os
import email

SUSSY_WORDS = ["Urgent", "Verify", "Account", "Password", "Day"]

def count_suspicious_words(subject, body, SUSSY_WORDS=SUSSY_WORDS):
    SusCount = {}
    subject_lower = subject.lower()
    body_lower = body.lower()
    for Kword in SUSSY_WORDS:
        count = subject_lower.count(Kword.lower()) + body_lower.count(Kword.lower())
        SusCount[Kword] = count
    return SusCount

def extract_subject_body(filepath: str):
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
            elif content_type == "text/html" and not body:
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

def analyze_folder(DATASET_FOLDER="datasets"):
    results = []
    for filename in os.listdir(DATASET_FOLDER):
        filepath = os.path.join(DATASET_FOLDER, filename)
        if os.path.isfile(filepath):
            subject, body = extract_subject_body(filepath)
            SussCounted = count_suspicious_words(subject, body)
            summary = ", ".join(f"{k}:{v}" for k, v in SussCounted.items())
            total = sum(SussCounted.values())
            results.append({
                "filename": filename,
                "subject": subject[:60],
                "total": total,
                "summary": summary
            })
    return results