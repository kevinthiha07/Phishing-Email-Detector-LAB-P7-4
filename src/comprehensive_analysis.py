import os
from email_processing import extract_subject_body
from whitelist import extract_email_from_content, whitelist_score
from keywords import keyword_detection_score
from keyword_position import keyword_position_score
#from edit_distance_check import check_edit_distance_with_risk_score
from url_detection import url_detection_score

def analyze_email_file(filepath, filename):
    """Comprehensive analysis of a single email file"""
    try:
        # Read email content for sender extraction
        with open(filepath, 'r', encoding='utf-8', errors='ignore') as f:
            content = f.read()
        
        # Extract sender email
        sender_email = extract_email_from_content(content)
        
        # Extract subject and body for analysis
        subject, body = extract_subject_body(filepath)
        
        # Calculate all scores
        whitelist_sc = whitelist_score(sender_email)
        keyword_detect_sc = keyword_detection_score(subject, body)
        keyword_pos_sc = keyword_position_score(subject, body)
        #edit_dist_sc = check_edit_distance_with_risk_score(sender_email)
        url_detect_sc = url_detection_score(subject, body, sender_email)
        
        # Calculate final score
        final_score = whitelist_sc + keyword_detect_sc + keyword_pos_sc  + url_detect_sc
        "+ edit_dist_sc"
        
        # Determine status
        if final_score < 50:
            status = "✅ Not Phishing"
            risk_level = "safe"
        elif final_score <= 74:
            status = "⚠️ Likely Phishing" 
            risk_level = "suspicious"
        else:
            status = "🚨 Definitely Phishing"
            risk_level = "phishing"
        
        return {
            "filename": filename,
            "subject": subject[:80] + "..." if len(subject) > 80 else subject,
            "sender": sender_email or "Unknown",
            "scores": {
                "whitelist": whitelist_sc,
                "keyword_detection": keyword_detect_sc,
                "keyword_position": keyword_pos_sc,
                #"edit_distance": edit_dist_sc,
                "url_detection": url_detect_sc
            },
            "final_score": final_score,
            "status": status,
            "risk_level": risk_level
        }
    
    except Exception as e:
        return {
            "filename": filename,
            "subject": "Error processing file",
            "sender": "Unknown",
            "scores": {"whitelist": 0, "keyword_detection": 0, "keyword_position": 0, "edit_distance": 0, "url_detection": 0},
            "final_score": 0,
            "status": f"❌ Error: {str(e)}",
            "risk_level": "error"
        }

def analyze_all_emails(dataset_folder="datasets"):
    """Analyze all emails in the dataset folder"""
    all_results = []
    
    if not os.path.exists(dataset_folder):
        return {"detailed_results": [], "summary_stats": {}, "all_results_count": 0}
    
    for filename in os.listdir(dataset_folder):
        filepath = os.path.join(dataset_folder, filename)
        if os.path.isfile(filepath):
            result = analyze_email_file(filepath, filename)
            all_results.append(result)
    
    # Sort by final score (descending)
    all_results.sort(key=lambda x: x['final_score'], reverse=True)
    
    # Get top 10 for detailed view
    detailed_results = all_results[:10]
    
    # Calculate summary statistics
    total_emails = len(all_results)
    safe_count = len([r for r in all_results if r['risk_level'] == 'safe'])
    suspicious_count = len([r for r in all_results if r['risk_level'] == 'suspicious'])
    phishing_count = len([r for r in all_results if r['risk_level'] == 'phishing'])
    
    # Calculate average scores
    avg_scores = {}
    if total_emails > 0 and detailed_results:
        for score_type in detailed_results[0]['scores'].keys():
            avg_scores[score_type] = round(sum(r['scores'][score_type] for r in all_results) / total_emails, 1)
        avg_final = round(sum(r['final_score'] for r in all_results) / total_emails, 1)
    else:
        avg_scores = {score_type: 0 for score_type in ['whitelist', 'keyword_detection', 'keyword_position', 'edit_distance', 'url_detection']}
        avg_final = 0
    
    summary_stats = {
        'total_emails': total_emails,
        'safe_count': safe_count,
        'suspicious_count': suspicious_count,
        'phishing_count': phishing_count,
        'avg_scores': avg_scores,
        'avg_final_score': avg_final
    }
    
    return {
        'detailed_results': detailed_results,
        'summary_stats': summary_stats,
        'all_results_count': total_emails
    }