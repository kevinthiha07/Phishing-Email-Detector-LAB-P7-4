from whitelist import whitelist_score
from keywords import keyword_score
from keyword_position import keyword_position_score
from edit_distance_check import edit_distance_score
from url_detection import url_score
from final_risk_scores import final_risk_score

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
