from flask import Flask, render_template
from keywords import analyze_folder
from whitelist import load_emails_from_dataset, whitelist_score, extract_email_from_content, load_whitelist
import os

app = Flask(__name__)

def get_comprehensive_analysis():
    """Get both keyword and whitelist analyses separately"""
    # Keyword analysis
    keyword_results = analyze_folder()
    
    # Whitelist analysis
    emails = load_emails_from_dataset("datasets")
    valid_emails = []
    suspicious_emails = []
    
    for email in emails:
        if email and '@' in email:
            score = whitelist_score(email)
            email_data = {
                'email': email,
                'status': 'Valid' if score == 0 else 'Suspicious',
                'score': score
            }
            if score == 0:
                valid_emails.append(email_data)
            else:
                suspicious_emails.append(email_data)
    
    return {
        'keyword_analysis': keyword_results,
        'valid_emails': valid_emails,
        'suspicious_emails': suspicious_emails,
        'whitelist_domains': list(load_whitelist()[0]),  # Convert set to list
        'whitelist_tlds': list(load_whitelist()[1])      # Convert set to list
    }

@app.route("/")
def newsletter():
    analysis_data = get_comprehensive_analysis()
    
    return render_template(
        "Test2.html",
        analysis_results=analysis_data['keyword_analysis'],
        valid_emails=analysis_data['valid_emails'],
        suspicious_emails=analysis_data['suspicious_emails'],
        whitelist_domains=analysis_data['whitelist_domains'],
        whitelist_tlds=analysis_data['whitelist_tlds'],
        total_emails=len(analysis_data['valid_emails']) + len(analysis_data['suspicious_emails'])
    )

if __name__ == "__main__":
    app.run(debug=True)