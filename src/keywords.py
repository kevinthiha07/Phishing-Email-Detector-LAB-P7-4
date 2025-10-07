import re

SUSPICIOUS_KEYWORDS = ["urgent", "verify", "account", "password", "click", "login", "bank", "update", "confirm"]

def keyword_detection_score(subject, body):
    """Count suspicious keywords - 15% weight"""
    text = f"{subject} {body}".lower()
    total_keywords = sum(text.count(keyword) for keyword in SUSPICIOUS_KEYWORDS)
    
    if total_keywords == 0:
        return 0
    elif 1 <= total_keywords <= 5:
        return 5
    elif 6 <= total_keywords <= 10:
        return 10
    else:  # >11 keywords
        return 15