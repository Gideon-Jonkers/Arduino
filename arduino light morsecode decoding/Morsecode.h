String Decode(String text)
{
  String letter;
  String rtext;// = "Morse code: ";
  bool space = false;
  for(unsigned int c = 0;c<text.length();c++)
  {
    space = false;
    for(int i = 0;i<5;i++)
    {
      if(text[c+i] == ' ')
      {
        for(int d = 0;d<i;d++){ letter+=text[c+d];}
        c+=i;
        goto let;
      }
      if(text[c+i] == '/')
      {
        for(int d = 0;d<i;d++) {letter+=text[c+d];}
        space = true;
        c+=i;
        goto let;
      }
    }
    let:
    unsigned long cval = 0;
    for(unsigned int i = 0;i<letter.length();i++)
    {
      cval += letter[i] == '_' ? (letter[i] + pow(2,i)) : (letter[i]);
      if(i == 0 && letter[i] == '_'){cval--;}
    }
    switch(cval)
    {
      case ('.' + '_' + 2):
        rtext += 'A';
      break;
      case ('_' + '.' + '.' + '.'):
        rtext += 'B';
      break;
      case ('_' + '.' + '_' + 4 + '.'):
        rtext += 'C';
      break;
      case ('_' + '.' + '.'):
        rtext += 'D';
      break;
      case ('.'):
        rtext += 'E';
      break;
      case ('.' + '.' + '_' + 4 + '.'):
        rtext += 'F';
      break;
      case ('_' + '_' + 2 + '.'):
        rtext += 'G';
      break;
      case ('.' + '.' + '.' + '.'):
        rtext += 'H';
      break;
      case ('.' + '.'):
        rtext += 'I';
      break;
      case ('.' + '_' + 2 + '_' + 4 + '_' + 8):
        rtext += 'J';
      break;
      case ('_' + '.'+ '_' + 4):
        rtext += 'K';
      break;
      case ('.' + '_' + 2 + '.' + '.'):
        rtext += 'L';
      break;
      case ('_' + '_' + 2):
        rtext += 'M';
      break;
      case ('_' + '.'):
        rtext += 'N';
      break;
      case ('_' + '_' + 2 + '_' + 4):
        rtext += 'O';
      break;
      case ('.' + '_' + 2 + '_' + 4 + '.'):
        rtext += 'P';
      break;
      case ('_' + '_' + 2 + '.' + '_' + 8):
        rtext += 'Q';
      break;
      case ('.' + '_' + 2 + '.'):
        rtext += 'R';
      break;
      case ('.' + '.' + '.'):
        rtext += 'S';
      break;
      case ('_'):
        rtext += 'T';
      break;
      case ('.' + '.' + '_' + 4):
        rtext += 'U';
      break;
      case ('.' + '.'+ '.' + '_' + 8):
        rtext += 'V';
      break;
      case ('.' + '_'+ 2 + '_' + 4):
        rtext += 'W';
      break;
      case ('_' + '.' + '.' + '_' + 8):
        rtext += 'X';
      break;
      case ('_' + '.' + '_' + 4 + '_' + 8):
        rtext += 'Y';
      break;
      case ('_' + '_'+ 2 +  '.' + '.'):
        rtext += 'Z';
      break;
    }
    if(space == true)
    {
      rtext += ' ';
    }
    letter = "";
  }
  return rtext;
}
