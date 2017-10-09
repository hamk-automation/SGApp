# SGApp - Visualization program for ESAV
SGApp - Suomi Genius Application (December 2016) - was created by team Suomi Genius to visualize scanned data of
road record file for ESAV project. The program visualizes the road surface by putting all scanned
data points together each colored depending on its height compared to the ground. Filtering function
was used to better reduce the error of reading data from SICK Tim 2D scanner. Further development of
this source code can be analysis of road record file used for different purposes such as traffic
engineering.

1. BUILD
    - Dependencies required to build SGApp from source:
        + wxWidgets
        + OpenGL
        + GLEW
    - To build SGApp from source, simply change to main directory of the source and type "make"

2. RUN
    Folder "in/" in the root folder contains samples taken from SGPiApp (link: https://github.com/hamk-automation/SGPiApp)
    that can be visualized by this application.
