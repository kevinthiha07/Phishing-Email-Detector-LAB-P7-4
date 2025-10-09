import Levenshtein
import re

# Known legitimate domains for comparison
KNOWN_DOMAINS = ["gmail.com", "yahoo.com", "outlook.com", "hotmail.com", "microsoft.com", "google.com", "lockergnome.com"]

def extract_domain(email: str) -> str:
    """Extract domain from an email address"""
    if not email or "@" not in email:
        return ""
    
    match = re.search(r'@([a-zA-Z0-9.-]+)', email)
    return match.group(1).lower() if match else ""

def calculate_edit_distance(str1: str, str2: str) -> int:
    """Calculate Levenshtein Edit Distance between two strings"""
    return Levenshtein.distance(str1, str2)

def edit_distance_score(email_address: str) -> int:
    """
    Calculate edit distance risk score - 20% weight
    
    Scoring:
    - Edit distance 1: 20% (high risk - lookalike domain)
    - Edit distance 2-3: 10% (medium risk)
    - Edit distance >3: 0% (low risk)
    - No domain found: 0%
    """
    if not email_address or '@' not in email_address:
        return 0
    
    domain = extract_domain(email_address)
    if not domain:
        return 0
    
    min_distance = float('inf')
    
    # Find the closest known domain
    for known_domain in KNOWN_DOMAINS:
        distance = calculate_edit_distance(domain, known_domain)
        if distance < min_distance:
            min_distance = distance
    
    if min_distance == 0:
        return 0    # Exact match - completely safe
    elif min_distance == 1:
        return 20   # Very similar domain
    elif min_distance <= 3:
        return 10   # Somewhat similar domain
    else:
        return 0    # Not similar enough to be suspicious

# For backward compatibility with your existing code
def check_edit_distance_with_risk_score(email_address: str) -> int:
    """Alias for edit_distance_score to maintain compatibility"""
    return edit_distance_score(email_address)

'''# Standalone testing (only runs if file is executed directly)
if __name__ == "__main__":
    # Test the function with some examples
    test_emails = [
        "user@gmail.com",           # Should return 0 (legitimate)
        "user@gma1l.com",           # Should return 20 (lookalike)
        "user@yahoo.com",           # Should return 0 (legitimate) 
        "user@yaho0.com",           # Should return 20 (lookalike)
        "user@outlook.com",         # Should return 0 (legitimate)
        "user@out1ook.com",         # Should return 20 (lookalike)
        "user@example.com",         # Should return 0 (not similar to known domains)
    ]
    
    print("Testing Edit Distance Scoring:")
    print("=" * 60)
    for email in test_emails:
        score = edit_distance_score(email)
        domain = extract_domain(email)
        print(f"Email: {email}")
        print(f"Domain: {domain} -> Risk Score: {score}%")
        print("-" * 40)'''