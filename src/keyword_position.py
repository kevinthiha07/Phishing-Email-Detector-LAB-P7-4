SUSPICIOUS_KEYWORDS = ["urgent", "verify", "account", "password", "click", "login", "bank", "update", "confirm", "spam", "free",
                       "now", "increase", "legal", "congratulations", "your", "asap", "help", "money", "chance", "offer", "fast","$",
                       "debt", "not spam"]

def keyword_position_score(subject, body):
    """Score based on keyword positions - 20% weight"""
    score = 0
    subject_lower = subject.lower()
    body_lower = body.lower()
    
    # Count unique keywords in each section to avoid over-penalizing repeated words
    subject_keywords = set()
    early_body_keywords = set()
    
    # Subject keywords: +5 points per unique keyword
    for keyword in SUSPICIOUS_KEYWORDS:
        if keyword in subject_lower:
            subject_keywords.add(keyword)
            score += 5
    
    # Early body keywords (first 200 chars): +3 points per unique keyword
    early_body = body_lower[:200]
    for keyword in SUSPICIOUS_KEYWORDS:
        if keyword in early_body:
            early_body_keywords.add(keyword)
            score += 3
    
    # Rest of body keywords: +1 point per unique keyword (not already counted in early body)
    rest_body = body_lower[200:]
    for keyword in SUSPICIOUS_KEYWORDS:
        if keyword in rest_body and keyword not in early_body_keywords:
            score += 1
    
    # Scoring based on weighted position points
    if score == 0:
        return 0
    elif score <= 3:
        return 5
    elif 4 <= score <= 8:
        return 10
    elif 9 <= score <= 15:
        return 15
    else:  # >15
        return 20