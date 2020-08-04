int properRainbowIncrement(float rgbArray[3]);
int inRange(float number, float start, float end);


int properRainbowIncrement(float rgbArray[3], int numLeds) {
  // LED RGB values in
  float red = rgbArray[0];
  float green = rgbArray[1];
  float blue = rgbArray[2];
  float x = 170;
  float length;
  float redMax;
  float redMin;
  float greenMax;
  float greenMin;
  float blueMax;
  float blueMin;
  float redStep;
  float greenStep;
  float blueStep;

  // NOTE: Do I need a num steps or something? Transition time?
  // Something to show how many steps for each section?

  // TODO: Do this is bytewise logic? It might be faster?
  // This probably doesn't matter.

  // break each section up into equal parts?
  // map current value to how many steps and figure out where you are?
  // use this to predict next value
  // 20:25

  if(inRange(red, 255, x) &&
     inRange(green, 0, 255 - x) &&
     inRange(blue, 0, 0)) {
    /********************************* SECTION 1 **********************************/
    length = 31 - 0;
    length = length * numLeds/255;
    redMax = 255;
    redMin = x;
    greenMax = 255 - x;
    greenMin = 0;
    blue = 0;

    redStep = (redMax - redMin)/length;
    red = red - redStep;
    if(red <= x) {
      red = x;
    }
    greenStep = (greenMax - greenMin)/length;
    green = green + greenStep;
    if(green > 255 - x) {
      green = 255 - x;
    }
    // std::cout << "Section 1" << std::endl;
       
  } else if(inRange(red, x, x) &&
	    inRange(green, 255 - x, x) &&
	    inRange(blue, 0, 0)) {
    /********************************* SECTION 2 **********************************/
    length = 63 - 32;
    length = length * numLeds/255;
    red = x;
    greenMax = x;
    greenMin = 255 - x;
    blue = 0;

    greenStep = (greenMax - greenMin)/length;
    green = green + greenStep;
    if(green > x) {
      green = x;
    }
    // std::cout << "Section 2" << std::endl;
    
  } else if(inRange(red, x, 0) &&
	    inRange(green, x, 255) &&
	    inRange(blue, 0, 0)) {
    /********************************* SECTION 3 **********************************/
    length = 95 - 64;
    length = length * numLeds/255;
    redMax = x;
    redMin = 0;
    greenMax = 255;
    greenMin = x;
    blue = 0;

    redStep = (redMax - redMin)/length;
    red = red - redStep;
    if(red < 0) {
      red = 0;
    }
    greenStep = (greenMax - greenMin)/length;
    green = green + greenStep;
    if(green > 255) {
      green = 255;
    }
    // std::cout << "Section 3" << std::endl;

  } else if(inRange(red, 0, 0) &&
	    inRange(green, 255, x) &&
	    inRange(blue, 0, 255 - x)) {
    /********************************* SECTION 4 **********************************/
    length = 127 - 96;
    length = length * numLeds/255;
    red = 0;
    greenMax = 255;
    greenMin = x;
    blueMax = 255 - x;
    blueMin = 0;

    greenStep = (greenMax - greenMin)/length;
    green = green - greenStep;
    if(green < x) {
      green = x;
    }
    blueStep = (blueMax - blueMin)/length;
    blue = blue + blueStep; 
    if(blue > 255 - x) {
      blue = 255 - x;
    }
    // std::cout << "Section 4" << std::endl;

  } else if(inRange(red, 0, 0) &&
	    inRange(green, x, 0) &&
	    inRange(blue, 255 - x, 255)) {
    /********************************* SECTION 5 **********************************/
    length = 159 - 128;
    length = length * numLeds/255;
    red = 0;
    greenMax = x;
    greenMin = 0;
    blueMax = 255;
    blueMax = x;

    greenStep = (greenMax - greenMin)/length;
    green = green - greenStep;
    if(green < 0) {
      green = 0;
    }
    blueStep = (blueMax - blueMin)/length;
    blue = blue + blueStep;
    if(blue > 255) {
      blue = 255;
    }
    // std::cout << "Section 5" << std::endl;

  } else if(inRange(red, 0, 255) &&
	    inRange(green, 0, 0) &&
	    inRange(blue, 255, 0)) {
    /********************************* SECTION 6 **********************************/
    length = 255 - 160;
    length = length * numLeds/255;
    redMax = 255;
    redMin = 0;
    green = 0;
    blueMax = 255;
    blueMin = 0;

    redStep = (redMax - redMin)/length;
    red = red + redStep;
    if(red > 255) {
      red = 255;
    }
    blueStep = (blueMax - blueMin)/length;
    blue = blue - blueStep;
    if(blue < 0) {
      blue = 0;
    }
    // std::cout << "Section 6" << std::endl;

  } else {
    /********************************* CATCH ALL **********************************/
    // Colors not in proper state
    // Put it in a state that can run
    red = 255;
    green = 0;
    blue = 0;
    // std::cout << "no matches" << std::endl;
    // std::cout << "red: \t" << rgbArray[0] << std::endl;
    // std::cout << "green: \t" << rgbArray[1] << std::endl;
    // std::cout << "blue: \t" << rgbArray[2] << std::endl << std::endl;
    rgbArray[0] = red;
    rgbArray[1] = green;
    rgbArray[2] = blue;
    return 1;
  }

  rgbArray[0] = red;
  rgbArray[1] = green;
  rgbArray[2] = blue;
  return 0;

}

int inRange(float number, float start, float end) {
  if(start == end) {
    return inRange(number, start+0.001, end-0.001);
    // return (number == start);
    
  } else if(end > start) {
    // Increasing
    // number is less than the end but greater than the start
    return (end > number && number >= start);
  } else if(start > end) {
    // Decreasing
    // If it's decreasing, start > end
    return (start >= number && number > end);
  } else {
    // everything has gone to hell
    // std::cout << "something went wrong in inRange" << std::endl;
    return false;
  }
}
