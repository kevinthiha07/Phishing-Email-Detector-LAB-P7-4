import re
import os
from urllib.parse import urlparse

DATASET_PATH = "datasets"
SAFE_DOMAINS = ["microsoft.com", "google.com", "apple.com", "amazon.com", "paypal.com", 
                "linkedin.com", "facebook.com", "instagram.com", "github.com", 
                "youtube.com", "cnet.com"]
SUSPICIOUS_TLDS = ['.xyz', '.top', '.club', '.click', '.link', '.online', '.tk', '.ml', '.ga', '.cf']

def extract_urls(text):
    url_pattern = r'https?://[^\s]+'
    urls = re.findall(url_pattern, text)
    return list(dict.fromkeys(urls))  # Remove duplicates

def get_domain(from_header):
    email = re.search(r'<([^>]+)>', from_header)
    email = email.group(1) if email else from_header
    return email.split('@')[-1].lower() if '@' in email else email.lower()

def is_suspicious_url(url, sender_domain):
    try:
        domain = urlparse(url).netloc.replace('www.', '')
        sender = sender_domain.replace('www.', '') if sender_domain else ''
        
        if re.match(r'^\d+\.\d+\.\d+\.\d+$', domain):
            return True, "IP address"
        if any(domain.endswith(tld) for tld in SUSPICIOUS_TLDS):
            return True, "suspicious TLD"
        if sender and domain != sender and domain not in [d.replace('www.', '') for d in SAFE_DOMAINS]:
            return True, "domain mismatch"
        
        return False, "safe URL"
    except Exception as e:
        return True, f"parsing error: {e}"

def read_email(file_path):
    try:
        with open(file_path, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
        
        lines = content.split('\n')
        headers = {}
        body_lines = []
        in_body = False
        
        for line in lines:
            if not in_body and not line.strip():
                in_body = True
                continue
            if not in_body and ':' in line:
                key, value = line.split(':', 1)
                headers[key.lower()] = value.strip()
            elif in_body:
                body_lines.append(line)
        
        return {
            'from': headers.get('from', ''),
            'subject': headers.get('subject', ''),
            'body': '\n'.join(body_lines),
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
    
    score = 25 if suspicious_urls else 0
    print(f"\nVerdict: {'SUSPICIOUS' if suspicious_urls else 'CLEAN'}")
    print(f"Suspicious URLs: {len(suspicious_urls)}/{len(urls)}")
    print(f"Score: {score}%")

def main():
    if not os.path.exists(DATASET_PATH):
        print(f"Dataset path not found: {DATASET_PATH}")
        return
    
    for filename in os.listdir(DATASET_PATH):
        filepath = os.path.join(DATASET_PATH, filename)
        if os.path.isfile(filepath):
            analyze_email(filepath)

if __name__ == "__main__":
    main()