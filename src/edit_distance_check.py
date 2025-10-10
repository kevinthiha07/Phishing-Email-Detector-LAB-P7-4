import Levenshtein
import re
import os
from collections import Counter

def extract_domain_counts(dataset_folder="datasets"):
    """
    Extract domains and their counts from dataset
    Returns Counter object with domain frequencies
    """
    if not os.path.exists(dataset_folder):
        return Counter()
    
    domains = []
    
    for filename in os.listdir(dataset_folder):
        filepath = os.path.join(dataset_folder, filename)
        if os.path.isfile(filepath):
            try:
                with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
                    content = f.read()
                    
                    # Extract email from From field
                    email_patterns = [
                        r'From:\s*[^<]*<([^>]+)>',
                        r'From:\s*([^\s<]+@[^\s>]+)',
                        r'Return-Path:\s*<([^>]+)>',
                    ]
                    
                    email_found = None
                    for pattern in email_patterns:
                        match = re.search(pattern, content)
                        if match:
                            email_found = match.group(1).strip()
                            break
                    
                    if not email_found:
                        email_matches = re.findall(r'\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Z|a-z]{2,}\b', content)
                        if email_matches:
                            email_found = email_matches[0]
                    
                    if email_found and '@' in email_found:
                        domain = email_found.split('@')[1].lower().strip()
                        if re.match(r'^[a-z0-9.-]+\.[a-z]{2,}$', domain):
                            domains.append(domain)
                            
            except Exception:
                continue
    
    domain_counts = Counter(domains)
    return domain_counts

def build_dynamic_known_domains(dataset_folder="datasets", min_frequency=2):
    """
    Build known domains dynamically from the dataset using frequency-based trust
    Domains that appear frequently in your dataset are considered legitimate
    """
    domain_counts = extract_domain_counts(dataset_folder)
    
    # Filter for domains that appear multiple times (likely legitimate due to frequency)
    frequent_domains = [domain for domain, count in domain_counts.items() 
                       if count >= min_frequency]
    
    # Static trusted domain list (well-known legitimate domains)
    STATIC_DOMAINS = [
        # Major Email Providers
        "gmail.com", "yahoo.com", "outlook.com", "hotmail.com", "aol.com",
        "icloud.com", "protonmail.com", "zoho.com", "mail.com", "msn.com",
        
        # Tech Companies
        "microsoft.com", "google.com", "apple.com", "amazon.com", 
        "facebook.com", "linkedin.com", "twitter.com", "instagram.com",
        "whatsapp.com", "telegram.org",
        
        # Financial Institutions
        "paypal.com", "visa.com", "mastercard.com", "americanexpress.com",
        "wellsfargo.com", "bankofamerica.com", "chase.com", "citibank.com",
        
        # Other High-Value Targets
        "netflix.com", "dropbox.com", "adobe.com", "salesforce.com",
        "spotify.com", "zoom.us", "skype.com",
        
        # Common legitimate domains from various countries
        "yahoo.co.uk", "yahoo.de", "yahoo.fr", "yahoo.jp",
        "web.de", "gmx.de", "gmx.net", "t-online.de",
        "naver.com", "daum.net", "qq.com", "163.com",
        
        # Add domains that you know are legitimate from your dataset , researched online
        # Add on more legit domains from dataset , if you know
        "rediff.com","ida.org","mx2.mail.yahoo.com","freemail.ru","msg.com","go.com"
    ]
    
    # Trust domains based on frequency (appear multiple times in your dataset)
    trusted_frequent_domains = []
    
    for domain in frequent_domains:
        # Skip if already in static list
        if domain in STATIC_DOMAINS:
            trusted_frequent_domains.append(domain)
            continue
            
        # Trust domains based on frequency thresholds
        count = domain_counts[domain]
        if count >= 2:  # Any domain appearing 2+ times is trusted
            trusted_frequent_domains.append(domain)
    
    # Combine with static known domains
    all_known_domains = list(set(STATIC_DOMAINS + trusted_frequent_domains))
    
    return all_known_domains

def extract_domain(email: str) -> str:
    """
    Extract domain from an email address
    """
    if not email or "@" not in email:
        return ""
    
    match = re.search(r'@([a-zA-Z0-9.-]+)', email)
    return match.group(1).lower() if match else ""

def calculate_edit_distance(str1: str, str2: str) -> int:
    """
    Calculate Levenshtein Edit Distance between two strings
    """
    return Levenshtein.distance(str1, str2)

def edit_distance_score(email_address: str) -> int:
    """
    Calculate phishing risk score based on domain similarity to known domains
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
    
    # Apply scoring rules
    if min_distance == 0:
        return 0    # Exact match - completely safe
    elif min_distance == 1:
        return 20   # Very similar domain - high risk
    elif min_distance <= 3:
        return 10   # Somewhat similar domain - medium risk
    else:
        return 0    # Not similar enough to be suspicious

def check_edit_distance_with_risk_score(email_address: str) -> int:
    """
    Compatibility wrapper for existing code
    """
    return edit_distance_score(email_address)

# Initialize dynamic known domains when module loads
KNOWN_DOMAINS = build_dynamic_known_domains()