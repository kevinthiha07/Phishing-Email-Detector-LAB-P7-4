#Hermann
import Levenshtein


#I think this can use a dataset instead of hard coding a list.
legitimate_domains = ["gmail.com", "yahoo.com", "outlook.com", "example.com"]
#To be replaced with a real email dataset.
#Need to find a way to use real life dataset.
email_data = [
    ("john.doe@gmail.con", "John Doe"),
    ("johndoe@gnail.com", "Johndoe"),
    ("jane_smith@yahoo.cm", "Jane Smith"),
]



# Function to check for similar fake domains
def check_similar_domains(email, legitimate_domains, threshold=2):
    """
    This function checks if the domain part of the email is similar to any legitimate domains.
    
    :param email: The email address to be checked.
    :param legitimate_domains: List of legitimate email domains.
    :param threshold: Maximum allowed Levenshtein distance for a domain to be considered suspicious.
    
    :return: List of domains that are suspiciously similar.
    """
    # Extract domain from the email (split by '@')
    domain = email.split('@')[-1]
    
    # List to store suspicious domains
    suspicious_domains = []
    
    # Compare the domain with legitimate domains
    for legit_domain in legitimate_domains:
        # Calculate Levenshtein distance
        distance = Levenshtein.distance(domain, legit_domain)
        
        # If the distance is within the threshold, consider it suspicious
        if distance <= threshold:
            suspicious_domains.append(legit_domain)
    
    return suspicious_domains


def check_similar_sender(sender_name, legitimate_names, threshold=2):
    """
    This function checks if the sender's name is visually similar to any legitimate names.
    
    :param sender_name: The name of the sender.
    :param legitimate_names: List of legitimate sender names.
    :param threshold: Maximum allowed Levenshtein distance for a name to be considered suspicious.
    
    :return: List of legitimate names that are suspiciously similar.
    """
    suspicious_names = []
    
    # Compare sender's name with legitimate sender names
    for legit_name in legitimate_names:
        # Calculate Levenshtein distance
        distance = Levenshtein.distance(sender_name.lower(), legit_name.lower())
        
        # If the distance is within the threshold, consider it suspicious
        if distance <= threshold:
            suspicious_names.append(legit_name)
    
    return suspicious_names


# Checking suspicious emails
for email, sender_name in email_data:
    suspicious_domains = check_similar_domains(email, legitimate_domains)
    suspicious_names = check_similar_sender(sender_name, ["John Doe", "Jane Smith", "Alice Johnson"])
    
    print(f"Email: {email}")
    if suspicious_domains:
        print(f"Suspicious domain(s): {', '.join(suspicious_domains)}")
    
    if suspicious_names:
        print(f"Suspicious sender name(s): {', '.join(suspicious_names)}")
    print("-" * 40)