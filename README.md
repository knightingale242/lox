# Souse

Souse is a dynamically-typed programming language, implemented from scratch in C. It comes with a **parser**, a **compiler**, and a **virtual machine (VM)** to interpret its instructions.

If that sounds familiar, it's because **Souse** takes inspiration from Robert Nystrom's *Lox*, described in his excellent book *Crafting Interpreters*. Lox served as a foundation, but Souse adds new functionality and optimizations to push the boundaries of what a small, elegant language can achieve.

Why "Souse"? Robert named his language after food, and I followed suit. "Souse" is a traditional Bahamian soup.

---

## Highlights

### **Implemented in C**
- Built for performance and fine-grained control.
- Includes all the major components: a **parser**, a **compiler**, and a **VM**.

### **Custom Memory Management**
- Souse uses a **custom implementation of malloc and free** for dynamic memory allocation. This provides greater control over memory usage and optimization.

### **Expanded Constant Limits**
- While Lox's constant pool maxes out at 256 constants, Souse supports **over 16 million constants** by expanding the representation by a byte.

### **Efficient Keyword Lookup**
- Keyword recognition is implemented using a **trie** structure instead of a series of `switch` statements.
- This results in faster lookups and cleaner code.

### **Improved Error Logs**
- Debugging is simpler with **clearer, more informative error messages**.
- Souse tells you *where* and *why* something went wrong, so you spend less time guessing and more time coding.

### **New Keywords**
- The **`in` keyword** makes it easy to check for membership in containers:
    ```c
    if ("apple" in fruits) {
        print "Apple is in the fruit list!";
    }
    ```

---

## Why Souse?
Souse is my personal exploration of language design and systems programming.
I made this for fun on a whim, so please don’t use it in any critical systems! If your website explodes or a plane falls out of the sky, I kindly ask you not to contact me :)

---

## What's Next?
Souse is a work in progress, with plenty of room for growth and improvement. Here are some ideas on the horizon:
- Additional keywords and features to make programming with Souse even more expressive.
- Static typing support (currently under consideration) for better performance and type safety.
- Concurrency Primitives and support

For now, enjoy Souse — a language rooted in *Crafting Interpreters* but refined with new ideas and a Bahamian twist.

---

## Acknowledgements
- A big thank you to Robert Nystrom for *Crafting Interpreters*, an invaluable resource for anyone interested in language design.
- I think everyone should read it and it might be the most useful and important CS book I have read next to OSTEP and CSAPP
---
