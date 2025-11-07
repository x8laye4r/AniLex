[![GitHub top language](https://img.shields.io/github/languages/top/x8laye4r/AniLex?logo=github)](https://github.com/x8laye4r/AniLex)
[![GitHub code size in bytes](https://img.shields.io/github/languages/code-size/x8laye4r/AniLex?logo=github)](https://github.com/x8laye4r/AniLex)
[![GitHub issues](https://img.shields.io/github/issues/x8laye4r/AniLex?logo=github)](https://github.com/x8laye4r/AniLex)
![GitHub repo size](https://img.shields.io/github/repo-size/x8laye4r/AniLex)

![GitHub stars](https://img.shields.io/github/stars/x8laye4r/AniLex?style=social)
![GitHub forks](https://img.shields.io/github/forks/x8laye4r/AniLex?style=social)


---

# AniLex – Your Personal Anime Tracker
First of all, I want to say it's **UNOFFICIAL** and not supported by **AniList**!

_**AniLex**_ is a native desktop app to manage your Anime, Manga, and Light Novel lists using the AniList API.  
It’s designed specifically for Windows users (for now) who want a clean, modern interface – no outdated UI, no web wrappers, no bloat.

> ⚠️ **Important:** This project is still **in active development**.  
> It's my **first major project in Python** and also my **very first desktop app**. I'm learning as I go, feedback is welcome!

> 🧪 **Note:** Currently only tested on **Windows**.  
> **Linux support is planned** once the core functionality is finished. <br>
> **macOS** might never come — I simply have no system to test it on.

> 💬 *Disclaimer:*  
> This is my **first major Python project** and also my **first desktop app**.  
> Things might break, explode, or summon undefined behavior while developing it until everything works as intended 

---

## 💡 Why AniLex?

I noticed that most AniList-based trackers either:
- Look outdated, especially on Windows (don't get me wrong they have cool features)
- Are web-only add-ons for Firefox or Chrome
- Lack a clean, intuitive UI

AniLex aims to fix that by being:
- 🪶 A native Windows app (no Electron, no browser)
- 🧼 Clean and modern UI
- ⚙️ Highly customizable
- 🎨 Built for users who like to tweak their setup, but also who want something out of the box 
- 📺 Optional built-in anime player (HiAnimez + MPV) with automatic AniList tracking
- 🧩 Plugin system for adding your own Python tools

---

## 🔧 Features (planned / in progress)

### Implemented 
- Some easy API-Calls to AniList

### In Progess 
- More advanced API-Calls with getting data and mutations
- Generall backend programming 

### Planned 
- Fully customizable Anime/Manga cards (choose what info is shown)
- User-built UI parts via Qt Designer
- Integration with MPV player (maybe even auto-tracking) 
- Theming & UI customization
- Multi-user management
- Custom tools to integrate  

---

## Used in this Project 
- 🐍 Python 3.13
- 🎨 PySide6 (Qt Wrapper)
- 🧠 AniList GraphQL API
- 💾 SQLite3/JSON (local cache)

---

## 📛 Naming

As a German programming student, I took inspiration from the word **"Lexikon"** (meaning encyclopedia or dictionary).  
Combining that with **"Ani"** for anime, I came up with **AniLex** – a name that reflects both structure and focus: an organized place for all your anime data.

---

## Local installtion 
```bash 
git clone https://github.com/x8laye4r/AniLex.git
cd AniLex
pip install -r requirements.txt
python main.py
```

---

## 🤝 Contributing

AniLex is a personal learning project, but I'm always happy if someone wants to contribute!

If you're interested in helping out – whether it's writing better code, improving the UI, suggesting features, or just fixing typos – feel free to open issues or send pull requests.

I'm still learning, so please be patient if I ask questions or need a bit of help understanding your code 😄

Any contribution, no matter how small, is welcome!

Contact me via this E-Mail:
**x8laye4r-github@proton.me**

