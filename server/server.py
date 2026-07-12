from flask import Flask, request, jsonify
from flask_cors import CORS
import sqlite3
import hashlib
import os

app = Flask(__name__)
CORS(app)  # needed for WebAssembly cross-origin requests



DB_PATH = os.path.join(os.path.dirname(os.path.abspath(__file__)), "quickmath.db")



def get_db():
    conn = sqlite3.connect(DB_PATH)
    conn.row_factory = sqlite3.Row
    return conn

def init_db():
    conn = get_db()
    conn.execute('''
        CREATE TABLE IF NOT EXISTS users (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            username TEXT UNIQUE NOT NULL,
            password_hash TEXT NOT NULL,
            created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
        )
    ''')
    conn.execute('''
        CREATE TABLE IF NOT EXISTS high_scores (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            user_id INTEGER NOT NULL,
            difficulty TEXT NOT NULL,
            high_score INTEGER DEFAULT 0,
            FOREIGN KEY (user_id) REFERENCES users(id),
            UNIQUE(user_id, difficulty)
        )
    ''')
    conn.commit()
    conn.close()

@app.route('/register', methods=['POST'])
def register():
    data = request.get_json(force=True, silent=True)
    if not data:
        return jsonify({'error': 'No JSON data received'}), 400

    username = data.get('username')
    password = data.get('password')

    if not username or not password:
        return jsonify({'error': 'Missing fields'}), 400

    password_hash = hashlib.sha256(password.encode()).hexdigest()

    conn = get_db()
    try:
        conn.execute('INSERT INTO users (username, password_hash) VALUES (?, ?)',
                     (username, password_hash))
        conn.commit()
        return jsonify({'message': 'User created'}), 201
    except sqlite3.IntegrityError:
        return jsonify({'error': 'Username already exists'}), 409
    finally:
        conn.close()

@app.route('/login', methods=['POST'])
def login():
    data = request.get_json(force=True, silent=True)
    if not data:
        return jsonify({'error': 'No JSON data received'}), 400

    username = data.get('username')
    password = data.get('password')

    if not username or not password:
        return jsonify({'error': 'Missing fields'}), 400

    password_hash = hashlib.sha256(password.encode()).hexdigest()

    conn = get_db()
    try:
        user = conn.execute('SELECT * FROM users WHERE username = ? AND password_hash = ?',
                            (username, password_hash)).fetchone()
        if not user:
            return jsonify({'error': 'Invalid credentials'}), 401

        scores = conn.execute(
            'SELECT difficulty, high_score FROM high_scores WHERE user_id = ?',
            (user['id'],)).fetchall()

        score_dict = {row['difficulty']: row['high_score'] for row in scores}

        return jsonify({
            'id': user['id'],
            'username': user['username'],
            'high_scores': {
                'basic':       score_dict.get('basic', 0),
                'advanced':    score_dict.get('advanced', 0),
                'derivatives': score_dict.get('derivatives', 0),
                'limits':      score_dict.get('limits', 0)
            }
        }), 200
    finally:
        conn.close()

@app.route('/highscore', methods=['POST'])
def update_highscore():
    data = request.get_json(force=True, silent=True)
    if not data:
        return jsonify({'error': 'No JSON data received'}), 400
    
    user_id = data.get('user_id')
    score = data.get('score')
    difficulty = data.get('difficulty')

    conn = get_db()
    try:
        conn.execute('''
            INSERT INTO high_scores (user_id, difficulty, high_score)
            VALUES (?, ?, ?)
            ON CONFLICT(user_id, difficulty)
            DO UPDATE SET high_score = MAX(high_score, excluded.high_score)
        ''', (user_id, difficulty, score))
        conn.commit()
        return jsonify({'message': 'Score updated'}), 200
    finally:
        conn.close()

@app.route('/highscores', methods=['GET'])
def get_highscores():
    conn = get_db()
    # Grabs the highest score achieved on ANY difficulty per user for a top leaderboard
    users = conn.execute('''
        SELECT users.username, MAX(high_scores.high_score) as high_score 
        FROM users 
        JOIN high_scores ON users.id = high_scores.user_id
        GROUP BY users.username
        ORDER BY high_score DESC 
        LIMIT 10
    ''').fetchall()
    conn.close()
    return jsonify([{'username': u['username'], 'high_score': u['high_score']} for u in users]), 200

if __name__ == '__main__':
    init_db()
    app.run(debug=True)