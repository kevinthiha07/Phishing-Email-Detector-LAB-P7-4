import re
from urllib.parse import urlparse

SAFE_DOMAINS = ["microsoft.com", "google.com", "apple.com", "amazon.com", "paypal.com", "linkedin.com"]
SUSPICIOUS_TLDS = ['.xyz', '.top', '.club', '.click', '.link', '.online', '.tk', '.ml', '.ga', '.cf']

def extract_urls(text):
    """Extract URLs from text"""
    url_pattern = r'https?://(?:[-\w.]|(?:%[\da-fA-F]{2}))+[/\w\.-]*\??[/\w\.-=&%]*'
    return re.findall(url_pattern, text)

def url_detection_score(subject, body, sender_email):
    """Detect suspicious URLs - 25% weight"""
    urls = extract_urls(f"{subject} {body}")
    
    if not urls:
        return 0
    
    sender_domain = sender_email.split('@')[1].lower() if sender_email and '@' in sender_email else ""
    
    suspicious_count = 0
    for url in urls:
        try:
            domain = urlparse(url).netloc.replace('www.', '')
            
            # Check for IP address
            if re.match(r'^\d+\.\d+\.\d+\.\d+$', domain):
                suspicious_count += 1
                continue
            
            # Check for suspicious TLDs
            for tld in SUSPICIOUS_TLDS:
                if domain.endswith(tld):
                    suspicious_count += 1
                    continue
            
            # Check for domain mismatch
            if sender_domain and domain != sender_domain.replace('www.', ''):
                if domain not in [d.replace('www.', '') for d in SAFE_DOMAINS]:
                    suspicious_count += 1
                    
        except Exception:
            suspicious_count += 1
    
    # If any suspicious URLs found, return 25%
    return 25 if suspicious_count > 0 else 0