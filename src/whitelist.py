import re
import os

WHITELIST_DOMAINS = {"gmail", "yahoo", "outlook", "hotmail", "mail"}
WHITELIST_TLDS = {"com", "org", "net", "edu", "gov", "co"}

def extract_domain(email):
    """Extract email domain from email"""
    if email and '@' in email:
        return email.split('@')[1].lower()
    else:
        return None

def is_valid_domain_format(domain):
    """Check if domain exists and contains a dot"""
    if not domain or '.' not in domain: 
        return False
    
    parts = domain.split('.')
    return len(parts) >= 2 and all(parts) and len(parts[-1]) >= 2

def is_domain_whitelisted(email, domains, tlds):
    """Check if the email domain whitelisted"""
    domain = extract_domain(email)
    if not domain or not is_valid_domain_format(domain): 
        return False
    
    parts = domain.split('.')
    if parts[0] not in domains: 
        return False
    
    tld_checks = [parts[-1]]  
    tld_checks.append(f"{parts[-2]}.{parts[-1]}" if len(parts) >= 3 else None)
    
    return any(tld in tlds for tld in tld_checks if tld)

def load_whitelist():
    """Return whitelisted domains and TLDs"""
    return WHITELIST_DOMAINS, WHITELIST_TLDS

def whitelist_score(email):
    """Calculate whitelist score - 20% weight"""
    domains, tlds = load_whitelist()
    if is_domain_whitelisted(email, domains, tlds):
        return 0  
    else:
        return 20

def extract_email_from_content(content):
    """Extract email from content using regular expression"""
    match = re.search(r'From:\s*[^<]*<([^>]+)>|From:\s*([^\s<]+@[^\s>]+)', content)
    if match and (match.group(1) or match.group(2)):
        return (match.group(1) or match.group(2)).strip()
    else:
        return None

def load_emails_from_dataset(dataset_path):
    """Load and extract emails from dataset files"""
    if not os.path.exists(dataset_path): 
        return []
    
    emails = []
    for filename in os.listdir(dataset_path):
        file_path = os.path.join(dataset_path, filename)
        if not os.path.isfile(file_path):
            continue
            
        try:
            with open(file_path, 'r', encoding='utf-8', errors='ignore') as file:
                content = file.read()
                email = extract_email_from_content(content)
                
                if not email:
                    email_patterns = re.findall(r'\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Z|a-z]{2,}\b', content)
                    email = email_patterns[0] if email_patterns else None
                
                if email: 
                    emails.append(email)
                    
        except Exception:
            pass
    
    return emails