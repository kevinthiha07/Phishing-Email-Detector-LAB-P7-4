
SUSPICIOUS_KEYWORDS = ["urgent", "verify", "account", "password", "click", "login", "bank", "update", "confirm", "spam", "free",
                       "now", "increase", "legal", "congratulations", "your", "asap", "help", "money", "chance", "offer", "fast","$",
                       "debt", "not spam"]

def keyword_detection_score(subject, body):
    """Count suspicious keywords - 15% weight""" """Subject and Body of email in Email_processing.py"""
    text = f"{subject} {body}".lower()
    # Checks all of the words in lowercase
    total_keywords = sum(text.count(keyword) for keyword in SUSPICIOUS_KEYWORDS)
    # Totals the number of keywords found in the text variable, which is the subject and body of the email

    if total_keywords == 0:
        return 0
    elif 1 <= total_keywords <= 5:
        return 5
    elif 6 <= total_keywords <= 10:
        return 10
    else:  # >11 keywords
        return 15