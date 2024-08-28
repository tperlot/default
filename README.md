# Origin
  I have been trying to learn on my own without schooling and started heavy with a Linux/Wayland/Vulkan combo in C++.  Trying to simplify things I opted to go for anything CC0 or MIT licensed where I could.  One of the big problems with Linux is that libraries aren't unified between distributions and many contain BSD-like licenses I don't really want to deal with.

# Licensing
  I preffer this license be used as CC0.  In looking up libraries I found out some countries don't allow CC0, so I have these files dual-licensed using the same terms as the popular STB library.

# What's Included?
  The first files I felt needed to be made were for an immediate mode UI and a 2D collision detection system.  These are meant to be used together, but are separated into their own folders so that I can add notes for anyone who is trying to learn C++ or to clarify the code without having walls of text in the comments.

# Requirements
  Safest bet is C++20 and clang at the moment.  I mostly used clang as I found the GCC defaults to be overly pedantic, even with some warnings turned off.

  Since I work with Vulkan most things use glm.

  While I considered writing my own windowing I opted to go for GLFW with most things as it includes some basic keyboard and mouse input, which Linux doesn't document very well.  If someone has suggestions for input devices I would like to move away from external libraries where possible.
