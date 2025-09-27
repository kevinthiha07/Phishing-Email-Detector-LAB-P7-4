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
    #check if domain and top level domain is empty, have at least 2 parts(domain,tld) and whether tld have at least 2 char
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
    #return 0 if domain is whitelisted (safe), 1 if suspicious
    if is_domain_whitelisted(email, domains, tlds):
        return 0  
    else:
        return 1

def extract_email_from_content(content):
    #extract email from each dataset by using regular expression
    match = re.search(r'From:\s*[^<]*<([^>]+)>|From:\s*([^\s<]+@[^\s>]+)', content)
    #return extracted email if found 
    if match and (match.group(1) or match.group(2)):
        return (match.group(1) or match.group(2)).strip()
    else:
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
                #extract email from dataset content
                email = extract_email_from_content(content)
                
                #match email format
                if not email:
                    email_patterns = re.findall(r'\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Z|a-z]{2,}\b', content)
                    email = email_patterns[0] if email_patterns else None
                
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
    
    for email in valid_emails:
        print("=" * 40)
        print(f"email: {email}")
        
        #check if email domain is whitelisted
        if is_domain_whitelisted(email, domains, tlds):
            print("status: Valid")
            valid_count += 1
        else:
            print("status: Suspicious")
            suspicious_count += 1
        
    print(f"Total emails processed: {len(valid_emails)}")
    print(f"Valid emails: {valid_count}")
    print(f"Suspicious emails: {suspicious_count}")

if __name__ == "__main__":
    main()