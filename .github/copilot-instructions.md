# Copilot Instructions 

## General Style
- Use 4 spaces for indentations 
- Prefer snake_case for variable and functions 
- Comment functions with docstrings
- Prefer readability and clarity over brevity (otherwise requested) 
- All function variables should be in one line if they fit otherwise use multiple lines

## Examples 

```python
def foo(bar: int, baz: str) -> None:
    # Some logic here 
```

## Project Context 
This repository is an open source desktop application built with PySide6. <br>
It uses the AniList API to display anime/manga data. <br>
It also has some APIs built in like SauceNAO, Tracemoe, H!Animez, Animechan, Image APIs and OpenAI as Chatbot <br>
Code should focus on:
- Clean separation between UI logic and data handling
- Multithreading/Async tasks for fetching data

## Type Hints & Safety 
- Always include type hints for all functions, parameter and return values 
- Use modern Python typing (PEP 604) — for example, use `int | None` instead of `Optional[int]`
- Always specify `-> None` for functions that don’t return anything.
- Never omit type hints — avoid `Any` unless absolutely necessary.

## Code style 
- Avoid global variables 
- Keep functions simple 