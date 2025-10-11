import re
from urllib.parse import urlparse

SAFE_DOMAINS = [# Major Email Providers
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
        "rediff.com","ida.org","mx2.mail.yahoo.com","freemail.ru","msg.com","go.com","ebay.com","grenet.fr","cwie.net", "tfn.net","linux.org","263.net",
        "web.com","napster.com","cutey.com","patra.com","hotp.com","yahoo.lv","ba.no",]

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