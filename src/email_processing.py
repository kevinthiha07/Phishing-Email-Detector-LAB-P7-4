import email

def extract_subject_body(filepath: str):
    """Extract subject and body from email file"""
    with open(filepath, "r", encoding="latin1", errors='ignore') as f:
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