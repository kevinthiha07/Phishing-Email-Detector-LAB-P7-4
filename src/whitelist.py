import os
import re

def extract_domain(email):
    #extract email domain from email
    if email and '@' in email:
        return email.split('@')[1].lower()
    else:
        return None

def is_valid_domain_format(domain):
    #check if domain exists and contains a dot
    if not domain or '.' not in domain: 
        return False
    
    #split domain into parts
   # Verify domain has valid structure (domain.tld)
    parts = domain.split('.')
    return len(parts) >= 2 and all(parts) and len(parts[-1]) >= 2

def is_domain_whitelisted(email, domains, tlds):
    #check if the email domain whitelisted
    #extract and validate domain format
    domain = extract_domain(email)
    if not domain or not is_valid_domain_format(domain): 
        return False
    
    #split domain into parts
    #check if main domain is whitelisted
    parts = domain.split('.')
    if parts[0] not in domains: 
        return False
    
    #create list of TLDs to check if it is in suspicious
    #add 2 tld together if there are 2(e.g: .com.sg)
    tld_checks = [parts[-1]]  
    tld_checks.append(f"{parts[-2]}.{parts[-1]}" if len(parts) >= 3 else None)
    
    return any(tld in tlds for tld in tld_checks if tld)

def load_whitelist():
    #whitelisted domains and top level domain
    return (
        {"gmail", "yahoo", "outlook", "hotmail", "mail"}, 
        {"com", "org", "net", "edu", "gov", "co"}          
    )

#connect to main_rules.py
def whitelist_score(email):
    domains, tlds = load_whitelist()
    #return 0% if domain is whitelisted (safe), 20% if suspicious
    if is_domain_whitelisted(email, domains, tlds):
        return 0  # Safe domain - 0%
    else:
        return 20  # Suspicious domain - 20%

def extract_email_from_content(content):
    """Extract sender email from email content with improved filtering"""
    # Try multiple patterns in order of reliability
    patterns = [
        # Pattern 1: From field with angle brackets (most reliable)
        r'From:\s*[^<]*<([^>]+@[^>]+)>',
        # Pattern 2: From field with email directly
        r'From:\s*([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})',
        # Pattern 3: Return-Path
        r'Return-Path:\s*<([^>]+@[^>]+)>',
        # Pattern 4: Reply-To
        r'Reply-To:\s*[^<]*<([^>]+@[^>]+)>',
        # Pattern 5: Sender
        r'Sender:\s*[^<]*<([^>]+@[^>]+)>',
    ]
    
    for pattern in patterns:
        match = re.search(pattern, content, re.IGNORECASE)
        if match:
            email = match.group(1).strip()
            # Validate it's actually an email address
            if re.match(r'^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$', email):
                return email
    
    # Fallback: find all emails and take the first valid one that's not a message ID
    all_emails = re.findall(r'[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}', content)
    for email in all_emails:
        # Skip obvious message IDs and file paths
        if (not re.search(r'[0-9a-f]{16,}', email) and  # Skip hex message IDs
            not re.search(r'\\|/', email) and           # Skip paths with slashes
            not email.startswith('0000') and            # Skip numeric IDs
            len(email.split('@')[0]) > 3):              # Skip very short usernames
            return email
    
    return None

def load_emails_from_dataset(dataset_path):
    #check if dataset path exists
    if not os.path.exists(dataset_path): 
        return []
    
    emails = []

    #retrieve file name and create file path
    for filename in os.listdir(dataset_path):
        file_path = os.path.join(dataset_path, filename)
        #check if it's a existing file 
        if not os.path.isfile(file_path):
            continue
            
        try:
            #read file content
            with open(file_path, 'r', encoding='utf-8', errors='ignore') as file:
                content = file.read()
                #extract email from dataset content using improved function
                email = extract_email_from_content(content)
                
                #add email to list
                if email: 
                    emails.append(email)
                    
        except Exception:
            pass
    
    return emails

def main():
    #load whitelisted domains and tld
    domains, tlds = load_whitelist()

    #dataset reference
    DATASET_FOLDER = "datasets"  
    
    #load emails from dataset
    emails = load_emails_from_dataset(DATASET_FOLDER)

    #check if email is empty, contains "@" and extra domain and tld
    valid_emails = [email for email in emails if email and '@' in email and is_valid_domain_format(email.split('@')[1])]
    
    valid_count = suspicious_count = 0
    total_percentage = 0
    
    for email in valid_emails:
        print("=" * 40)
        print(f"email: {email}")
        
        #calculate whitelist percentage for this email
        percentage = whitelist_score(email)
        
        #check if email domain is whitelisted
        if is_domain_whitelisted(email, domains, tlds):
            print("status: Valid")
            print(f"whitelist score: {percentage}%")
            valid_count += 1
        else:
            print("status: Suspicious")
            print(f"whitelist score: {percentage}%")
            suspicious_count += 1
            total_percentage += percentage
        
    print("=" * 40)
    print(f"Total emails processed: {len(valid_emails)}")
    print(f"Valid emails: {valid_count}")
    print(f"Suspicious emails: {suspicious_count}")

if __name__ == "__main__":
    main()

