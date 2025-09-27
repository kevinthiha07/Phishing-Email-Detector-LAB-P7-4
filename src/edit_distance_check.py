import Levenshtein
import os
import re
from typing import List

# Folder path where the files are stored
folder_path = r'C:\Users\carol\Documents\Python\Phishing-Email-Detector-LAB-P7-4-1\datasets'

# Function to extract emails from a file
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
    return bool(re.match(r'^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$', email))

# Function to filter out non-email content (HTML or similar)
def is_html_like(line: str) -> bool:
    return bool(re.search(r'<[^a-zA-Z0-9._%+-]+>', line))

# Function to extract domain from an email
def extract_domain(email: str) -> str:
    if "@" in email and re.match(r'^[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\.[a-zA-Z]{2,}$', email):
        match = re.search(r'@([a-zA-Z0-9.-]+)', email)
        if match:
            return match.group(1)
    return ""

# Function to process multiple files in the folder
def process_files_in_folder(folder_path: str) -> List[str]:
    all_emails = []
    for filename in os.listdir(folder_path):
        file_path = os.path.join(folder_path, filename)
        if os.path.isfile(file_path):
            print(f"Processing file: {filename}")
            emails = extract_emails_from_file(file_path)
            all_emails.extend(emails)
    return all_emails

# Function to calculate Levenshtein Edit Distance
def calculate_edit_distance(str1: str, str2: str) -> int:
    return Levenshtein.distance(str1, str2)

# Function to compare extracted domains with known legitimate domains and find the most similar one
def check_edit_distance(emails: List[str], known_domains: List[str]) -> None:
    for email in emails:
        domain = extract_domain(email)  # Extract the full domain from the email
        
        if domain:
            # Standardize domain comparison (strip spaces and lowercase)
            domain = domain.strip().lower()

            # Find the known domain with the smallest Levenshtein distance
            most_similar_domain = None
            min_distance = float('inf')  # Start with a large value for comparison

            # Loop through the known domains to find the most similar one
            for known_domain in known_domains:
                known_domain = known_domain.strip().lower()

                # Calculate the Levenshtein distance between the email domain and known domain
                distance = calculate_edit_distance(domain, known_domain)

                # If this domain is more similar, update the most_similar_domain and min_distance
                if distance < min_distance:
                    most_similar_domain = known_domain
                    min_distance = distance

            # Print the result for the most similar domain
            if most_similar_domain is not None:
                print(f"Email: {email} -> Domain: {domain} | Most Similar Known Domain: {most_similar_domain} | Edit Distance: {min_distance}")

# Testing the function to process files and compare domains
emails = process_files_in_folder(folder_path)

# Define known legitimate domains
known_domains = ["gmail.com", "yahoo.com", "outlook.com", 'newsletter.online.com', 'lockergnome.com', 'sprocket.lockergnome.com']

# Check the edit distance between extracted domains and known domains
check_edit_distance(emails, known_domains)
