SUSPICIOUS_KEYWORDS = ["urgent", "verify", "account", "password", "click", "login", "bank", "update", "confirm", "spam", "free",
                       "now", "increase", "legal", "congratulations", "your", "asap", "help", "money", "chance", "offer", "fast","$",
                       "debt", "not spam"]

def keyword_position_score(subject, body):
    """Score based on keyword positions - 20% weight"""
    score = 0
    subject_lower = subject.lower()
    body_lower = body.lower()
    
    # Subject keywords: +5 points
    for keyword in SUSPICIOUS_KEYWORDS:
        if keyword in subject_lower:
            score += 5
    
    # Early body keywords (first 200 chars): +3 points
    early_body = body_lower[:200]
    for keyword in SUSPICIOUS_KEYWORDS:
        if keyword in early_body:
            score += 3
    
    # Rest of body keywords: +1 point
    for keyword in SUSPICIOUS_KEYWORDS:
        if keyword in body_lower[200:]:
            score += 1
    
    if score <= 5:
        return 5
    elif 6 <= score <= 15:
        return 10
    else:  # >15
        return 20