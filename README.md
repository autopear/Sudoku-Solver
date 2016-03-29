# Sudoku Solver
CIS 5603 course project

I will try to add more details.

This is a small Sudoku board defined in SudokuBoard class.
<img src="https://github.com/autopear/Sudoku-Solver/blob/master/Documents/Images/01.png">

Every cell is identified with an X value and Y value, which is the row and column of the cell, starting from 0. So the cell on the top left corner is (0, 0).

An inner block is defined with a list of cells. For example, in the above picture, a block with cell (0, 0), (0, 1), (1, 0) and (1, 1). Every cell in a block should appear in the list. Block can have any shape other than rectangles, so irregular shapes are allowed, though I don't know whether we will deal with them or not.

To specify a board, add a text file with .board extension into Resources/Boards directory. The file format is specified below:
<img src="https://github.com/autopear/Sudoku-Solver/blob/master/Documents/Images/02.png">

Every board has 6 keys:<ul>
<li><b>NAME</b>: This key must be unique, which is the name of the variant.</li>
<li><b>ROWS</b> and <b>COLUMNS</b>: Specify the grid size of the board. These two values can be different.</li>
<li><b>MINIMUM</b> and <b>MAXIMUM</b>: Define the range of allowed value numbers. Normally it should correspond to the grid size. A valid value must be at least 1. 0 is used as undefined value.</li>
<li><b>BLOCKS</b>: Specify all the blocks in the grid. You can put multiple blocks in one line. Every block is surrounded by [ and ], two blocks are separated by a comma. One cell can appear at most once in all blocks. A cell is in the format of (X, Y) and two cells are separated by comma. The end of the block list must be semicolon.</li>
<li><b>COLORS</b>: Specify the colors of each block. The color can be either normal string or HTML color code.<ol>
  <li>Color as string: <a href="http://doc.qt.io/qt-5/qcolor.html#predefined-colors" target="_blank">http://doc.qt.io/qt-5/qcolor.html#predefined-colors</a></li>
  <li>HTML color code: <a href="http://htmlcolorcodes.com/" target="_blank">http://htmlcolorcodes.com/</a></li>
</ol></li>
</ul>

User can also have preset (initial values) defined for a specified board. The file must have extension .sdk.
<img src="https://github.com/autopear/Sudoku-Solver/blob/master/Documents/Images/03.png">

The sdk file is basically a 2-D arrays of integers, with values corresponding to the grid. Undefined (empty) values can be set to empty string or 0. Rows of values must be in individual lines, and values in the same row are separated by comma. The above preset is for grid 9x9. An example output is below:
<img src="https://github.com/autopear/Sudoku-Solver/blob/master/Bin/Preview_9x9_Presets.png">


<b>How to compile</b>
<ul>	  
<li><p>Mac OS X Requirements: Xcode (from Mac App Store) and Qt</p></li>

<li><p>Windows Requirements: Visual C++ Compiler and Qt<ul>
	<li>Visual C++ Compiler is integrated in Visual Studio (including most versions, Ultimate, Pro, Express, etc.).</li>
	<li>Visual Studio needs to be 2010, 2012, 2013 or 2015.</li></ul></p></li>

<li><p>Qt Installer: <a href="http://www.qt.io/download-open-source/#section-2">http://www.qt.io/download-open-source/#section-2</a></p></li>
  
<li><p>What to install<br/>In the Select Components page of the installer, select the following components:<ul>
<li>Mac OS X: Qt->Qt 5.6->clang 64-bit</li>
<li>Windows: Qt->Qt 5.6->MSVC 201X, depending on the version of you Visual Studio. You can install both 32 bit and 64 bit versons. But in general 32 bit is enough.</li>
<li>Qt->Tools->Qt Creator (this is forced selected)</li>
<li>You can select other components, but they are unnecessary for this project.</li></ul></p></li>
<li><p>How to compile<br/>Use Qt Creator to open the .pro file, and select a suitable Kit for the project. Then you can build or rebuild the project. Sometimes you need to "Run qmake" before building the project. If you are developing, then choose Debug as build configuration in the Projects tab on the left tool bar. This gives you the ability to track functions and set breakpoints.</li>
</li></ul>