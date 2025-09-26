import os
import re

def extract_domain(email):
    """Extract the domain part from an email address."""
    if email and '@' in email:
        return email.split('@')[1].lower()
    else:
        return None

def is_valid_domain_format(domain):
    """Check if the domain has a valid format (e.g., domain.tld)."""
    # Check if domain exists and contains a dot
    if not domain or '.' not in domain: 
        return False
    
    # Split domain into parts and validate
    parts = domain.split('.')
    return len(parts) >= 2 and all(parts) and len(parts[-1]) >= 2

def is_domain_whitelisted(email, domains, tlds):
    """Check if the email domain is in the whitelist."""
    # Extract and validate domain format
    domain = extract_domain(email)
    if not domain or not is_valid_domain_format(domain): 
        return False
    
    # Split domain into parts
    parts = domain.split('.')
    
    # Check if main domain (e.g., 'gmail' from 'gmail.com') is whitelisted
    if parts[0] not in domains: 
        return False
    
    # Create list of TLDs to check (single and two-part TLDs)
    tld_checks = [parts[-1]]  # Single TLD (e.g., 'com')
    
    # Add two-part TLD if available (e.g., 'co.uk')
    if len(parts) >= 3:
        tld_checks.append(f"{parts[-2]}.{parts[-1]}")
    else:
        tld_checks.append(None)
    
    # Return True if any TLD version is in the whitelist
    return any(tld in tlds for tld in tld_checks if tld)

def load_whitelist():
    """Load the whitelisted domains and TLDs."""
    return (
        {"gmail", "yahoo", "outlook", "hotmail", "mail"},  # Approved domains
        {"com", "org", "net", "edu", "gov", "co"}          # Approved TLDs
    )

def extract_email_from_content(content):
    """Extract email address from email header content."""
    # Search for email pattern in From: header
    match = re.search(r'From:\s*[^<]*<([^>]+)>|From:\s*([^\s<]+@[^\s>]+)', content)
    
    # Return extracted email if found, otherwise None
    if match and (match.group(1) or match.group(2)):
        return (match.group(1) or match.group(2)).strip()
    else:
        return None

def load_emails_from_dataset(dataset_path):
    """Load and extract emails from all files in the dataset directory."""
    # Check if dataset path exists
    if not os.path.exists(dataset_path): 
        return []
    
    emails = []
    
    # Process each file in the dataset directory
    for filename in os.listdir(dataset_path):
        file_path = os.path.join(dataset_path, filename)
        
        try:
            # Read file content
            with open(file_path, 'r', encoding='utf-8', errors='ignore') as file:
                content = file.read()
                
                # Try to extract email from From: header first
                email = extract_email_from_content(content)
                
                # If no email found in header, search for any email pattern in content
                if not email:
                    email_patterns = re.findall(r'\b[A-Za-z0-9._%+-]+@[A-Za-z0-9.-]+\.[A-Z|a-z]{2,}\b', content)
                    if email_patterns:
                        email = email_patterns[0]  # Use first email found
                
                # Add email to list if found
                if email: 
                    emails.append(email)
                    
        except Exception as e:
            # Skip files that can't be read
            pass
    
    return emails

def main():
    """Main function to run the email safety checker."""
    # Load whitelisted domains and TLDs
    domains, tlds = load_whitelist()
    
    # Define path to dataset directory
    dataset_path = r"C:\Users\User\OneDrive\Documents\Phishing-Email-Detector-LAB-P7-4\datasets"
    
    # Load emails from dataset files
    emails = load_emails_from_dataset(dataset_path)
    
    # Filter valid email formats
    valid_emails = [
        email for email in emails 
        if email and '@' in email and is_valid_domain_format(email.split('@')[1])
    ]
    
    # Initialize counters
    valid_count = 0
    suspicious_count = 0
    
    # Process each valid email
    for email in valid_emails:
        print("=" * 40)
        print(f"email: {email}")
        
        # Check if email domain is whitelisted
        if is_domain_whitelisted(email, domains, tlds):
            print("status: Valid")
            valid_count += 1
        else:
            print("status: Suspicious")
            suspicious_count += 1
        
        print()  # Empty line for separation
    
    # Print summary statistics
    print(f"Total emails processed: {len(valid_emails)}")
    print(f"Valid emails: {valid_count}")
    print(f"Suspicious emails: {suspicious_count}")

if __name__ == "__main__":
    main()