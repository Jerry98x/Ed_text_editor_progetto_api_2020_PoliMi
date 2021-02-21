Ed (Standard Unix text editor)

C project developed as part of the API course examination at Politecnico di Milano (2019/2020).
This C program simulates text editor "Ed" (or "EdU") for Unix operative systems, which uses an index to represent th current line.

Text commands:
  - (ind1,ind2)c
      Change text from line 'ind1' to line 'ind2'. 'ind1' must be an already written line in the document or the first line after the last written line.
  - (ind1,ind2)d
      Delete text from line 'ind1' to line 'ind2'. The lines from 'ind2+1' and on will then be shifted left to fill the blank lines.
  - (ind1,ind2)p
      Print text from line 'ind1' to line 'ind2'. If a line is blank, the dot character '.' will be printed.
  - (number)u
      Undo the previous 'number' c-commands or d-commands
  - (number)r
      Redo the last 'number' c-commands and d-commands after a u-command has been done
  - q
      Ends text editor execution.


A maximum of 1024 characters can be written in a single command.
This program assumes that 'ind2' is always greater than 'ind1' and the two parameters can't be incorrect.
