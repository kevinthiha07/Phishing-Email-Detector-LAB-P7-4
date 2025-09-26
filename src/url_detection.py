import re
import os
import glob
from urllib.parse import urlparse

DATASET_PATH = r"C:\Users\Sharl\Documents\Programming\Phishing-Email-Detector-LAB-P7-4\datasets"

SAFE_DOMAINS = ["microsoft.com", "google.com", "apple.com", "amazon.com", "paypal.com", 
                "linkedin.com", "facebook.com", "instagram.com", "github.com", 
                "youtube.com", "cnet.com"]

SUSPICIOUS_TLDS = ['.xyz', '.top', '.club', '.click', '.link', '.online', '.tk', '.ml', '.ga', '.cf']

def extract_urls(text):
    url_pattern = r'https?://(?:[-\w.]|(?:%[\da-fA-F]{2}))+[/\w\.-]*\??[/\w\.-=&%]*'
    return re.findall(url_pattern, text)

def get_domain(from_header):
    email_match = re.search(r'<([^>]+)>', from_header)
    email = email_match.group(1) if email_match else from_header
    return email.split('@')[1].lower() if '@' in email else email.lower()

def is_suspicious_url(url, sender_domain):
    try:
        domain = urlparse(url).netloc.replace('www.', '')
        sender_clean = sender_domain.replace('www.', '') if sender_domain else ''
        
        # check for IP address
        if re.match(r'^\d+\.\d+\.\d+\.\d+$', domain):
            return True, "IP address"
        
        # check for suspicious TLDs
        for tld in SUSPICIOUS_TLDS:
            if domain.endswith(tld):
                return True, "suspicious TLD"
        
        # check for domain mismatch
        safe_domains_clean = [d.replace('www.', '') for d in SAFE_DOMAINS]
        if sender_domain and domain != sender_clean and domain not in safe_domains_clean:
            return True, "domain mismatch"
        
        return False, "safe URL"
        
    except Exception as e:
        return True, f"parsing error: {e}"

def read_email(file_path):
    try:
        with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
        
        # extract email headers and body
        headers = {}
        body = ""
        lines = content.split('\n')
        header_end_found = False
        current_line = 0
        
        while current_line < len(lines) and not header_end_found:
            line = lines[current_line]
            if not line.strip():  # Empty line indicates end of headers
                body = '\n'.join(lines[current_line + 1:])
                header_end_found = True

            elif ':' in line:
                key, value = line.split(':', 1)
                headers[key.lower()] = value.strip()
            current_line += 1
        
        return {
            'from': headers.get('from', ''),
            'subject': headers.get('subject', ''),
            'body': body,
            'filename': os.path.basename(file_path)
        }
    except Exception as e:
        print(f"Error reading {file_path}: {e}")
        return None

def analyze_email(file_path):
    email = read_email(file_path)
    if not email:
        return
    
    print(f"\n{'='*50}")
    print(f"Analyzing: {email['filename']}")
    print(f"{'='*50}")
    print(f"From: {email['from']}")
    print(f"Subject: {email['subject']}")
    
    sender_domain = get_domain(email['from'])
    urls = extract_urls(email['body'])
    suspicious_urls = []
    
    print(f"\nFound {len(urls)} URLs:")
    
    url_count = 1
    for url in urls:
        is_suspicious, reason = is_suspicious_url(url, sender_domain)
        status = "SUSPICIOUS" if is_suspicious else "SAFE"
        
        print(f"{url_count}. [{status}] {url}")
        print(f"   Reason: {reason}")
        
        if is_suspicious:
            suspicious_urls.append(url)
        
        url_count += 1
    
    # verdict of email
    print(f"\nVerdict: {'SUSPICIOUS' if suspicious_urls else 'CLEAN'}")
    print(f"Suspicious URLs: {len(suspicious_urls)}/{len(urls)}")

def main():
    if not os.path.exists(DATASET_PATH):
        print(f"Dataset path not found: {DATASET_PATH}")
        return
    
    # List all files in dataset path
    files = [f for f in glob.glob(os.path.join(DATASET_PATH, '*')) if os.path.isfile(f)]
    
    if not files:
        print("No files found!")
        return
    
    # Analyze ALL files automatically
    for file_path in files:
        analyze_email(file_path)
        
if __name__ == "__main__":
    main()