This is a little Connect Four project. It uses machine learning to evaluate the moves

Folder structure:

    - bin: The output executables go here, both for the app and for any tests and spikes.

    - build: This folder contains all object files, and is removed on a clean.

    - doc: Any notes go here.

    - include: All project header files. All necessary third-party header files that do not exist under /usr/local/include are also placed here.

    - lib: Any libs that get compiled by the project, third party or any needed in development. Prior to deployment, third party libraries get moved to /usr/local/lib where they belong, leaving the project clean enough to compile on our Linux deployment server. (We should only use St-librarys, so maybe we can leave this folder empty)

    - spike: It's for little test files, to test new technologie and so on. 

    - src: The application and only the application’s source files.

    - test: Our unit tests.
