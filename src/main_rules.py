from flask import Flask, render_template
from whitelist import load_emails_from_dataset, whitelist_score, extract_email_from_content, load_whitelist
from keywords import keyword_score
from keyword_position import keyword_position_score
from edit_distance_check import edit_distance_score
from url_detection import url_score
from final_risk_scores import final_risk_score
from keywords import analyze_folder

app = Flask(__name__)

def score_email(email):
    scores = {
        "whitelist": whitelist_score(email),
        "keyword_detection": keyword_score(email),
        "keyword_position": keyword_position_score(email),
        "edit_distance": edit_distance_score(email),
        "url_detection": url_score(email),
        "final_risk_score": final_risk_score(email),
    }

    # final = sum(scores.values())
    # label = "Phishing" if final >= 5 else "Safe"
    # return final, label, scores

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
        'whitelist_domains': load_whitelist()[0],  # Get the domains set
        'whitelist_tlds': load_whitelist()[1]      # Get the TLDs set
    }

@app.route("/")
def newsletter():
    analysis_data = get_comprehensive_analysis()
    
    most_popular = [
        {"title": "Dell Dimension 8000", "url": "#"},
        {"title": "Toshiba Sat. 5105-S607", "url": "#"},
        {"title": "Canon PowerShot G2", "url": "#"},
        {"title": "Dell Latitude C400", "url": "#"},
        {"title": "Nikon Coolpix 995", "url": "#"},
    ]
    
    return render_template(
        "Test2.html",
        most_popular=most_popular,
        analysis_results=analysis_data['keyword_analysis'],
        valid_emails=analysis_data['valid_emails'],
        suspicious_emails=analysis_data['suspicious_emails'],
        whitelist_domains=analysis_data['whitelist_domains'],
        whitelist_tlds=analysis_data['whitelist_tlds'],
        total_emails=len(analysis_data['valid_emails']) + len(analysis_data['suspicious_emails'])
    )
if __name__ == "__main__":
    app.run(debug=True)