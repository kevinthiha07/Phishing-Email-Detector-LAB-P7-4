#Hermann
import Levenshtein
import os 
import re
from collections import Counter
from typing import List


folder_path = r'C:\Users\Admin\Documents\Python\Phishing-Email-Detector-LAB-P7-4\datasets'
#print(f"Folder path: {folder_path}")
file_path = r'C:\Users\Admin\Documents\Python\Phishing-Email-Detector-LAB-P7-4\datasets\0001.f0cf04027e74802f09f723cb8916b48e'
#print(f"Folder path: {file_path}")




def extract_emails_from_file(file_path: str) -> List[str]:
    emails = []
    seen_domains = set()  # To track domains already processed
    with open(file_path, 'r', encoding='utf-8', errors='ignore') as file:
        lines = file.readlines()
        for line in lines:
            if 'From: ' in line or 'Return-Path:' in line:
                match = re.search(r'<([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})>', line)
                if match:
                    email = match.group(1)
                    if validate_email(email):
                        domain = extract_domain(email)
                        if domain not in seen_domains:
                            seen_domains.add(domain)
                            emails.append(email)
                else:
                    match = re.search(r'([a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,})', line)
                    if match:
                        email = match.group(1)
                        if not is_html_like(line) and validate_email(email):
                            domain = extract_domain(email)
                            if domain not in seen_domains:
                                seen_domains.add(domain)
                                emails.append(email)
    return emails

# Basic email validation function
def validate_email(email: str) -> bool:
    # A simple regex to validate the general email format
    return bool(re.match(r'^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$', email))

# Function to filter out non-email content (HTML or similar)
def is_html_like(line: str) -> bool:
    # This checks if the line contains HTML-like tags
    return bool(re.search(r'<[^a-zA-Z0-9._%+-]+>', line))  # Check for HTML tags


def extract_domain(email: str) -> str:
    # Check if the string contains an @ symbol and matches a general email pattern
    if "@" in email and re.match(r'^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$', email):
        match = re.search(r'@([a-zA-Z0-9.-]+)', email)
        if match:
            return match.group(1)
    return ""  # Return empty string if not a valid email


#Testing of code
emails = extract_emails_from_file(file_path)
print("Extracted Emails:", emails)

for email in emails:
    domain = extract_domain(email)
    if domain:
        print(f"Email: {email} -> Domain: {domain}")
    else:
        print(f"Email: {email} -> Invalid or not an email")
