from whitelist import whitelist_score
from keywords import keyword_score
from keyword_position import keyword_position_score
from edit_distance_check import edit_distance_score
from url_detection import url_score
from final_risk_scores import final_risk_score
from flask import Flask, render_template
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

@app.route("/")
def newsletter():
    analysis_results = analyze_folder()  # This collects the results
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
        analysis_results=analysis_results,
    )

if __name__ == "__main__":
    app.run(debug=True)