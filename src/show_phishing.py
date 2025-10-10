from comprehensive_analysis import analyze_all_emails
from edit_distance_check import get_edit_distance_details

#used for debugging 
#prints only phishing email , along with how edit distance is done.

def show_phishing_with_edit_distance():
    """Show phishing emails with edit distance comparison details"""
    print("🔍 Analyzing emails for phishing...")
    analysis_results = analyze_all_emails()
    
    # Filter only phishing emails
    phishing_emails = []
    for email in analysis_results['all_emails_results']:
        if email['risk_level'] == 'phishing':
            phishing_emails.append(email)
    
    print(f"\n{'='*80}")
    print(f"🚨 PHISHING EMAILS FOUND: {len(phishing_emails)}")
    print(f"{'='*80}")
    
    if not phishing_emails:
        print("No phishing emails detected!")
        return
    
    for i, email in enumerate(phishing_emails, 1):
        print(f"\n{i}. 📧 {email['filename']}")
        print(f"   📋 Subject: {email['subject']}")
        print(f"   👤 Sender: {email['sender']}")
        print(f"   ⚠️  Final Score: {email['final_score']}")
        print(f"   🚩 Status: {email['status']}")
        
        # Get edit distance details for this sender
        if email['sender'] and email['sender'] != 'Unknown':
            edit_details = get_edit_distance_details(email['sender'])
            print(f"   🔍 Edit Distance Analysis:")
            print(f"      - Closest legitimate domain: {edit_details.get('closest_domain', 'None')}")
            print(f"      - Distance: {edit_details.get('distance', 0)}")
            print(f"      - Score: {edit_details.get('score', 0)}")
        
        # Show all scores
        print(f"   📊 All Scores:")
        for score_type, score_value in email['scores'].items():
            print(f"      - {score_type}: {score_value}")
        
        print(f"{'-'*80}")

if __name__ == "__main__":
    show_phishing_with_edit_distance()