
void readJoystickValues() {
  //We read the values for each axis from the joystick
  xValue = analogRead(pinX);
  yValue = analogRead(pinY);
}


//we check in which direction the movement of the joysick is
void checkDirection() {

  //Right direction
  if (xValue > maxThreshold && joyMoved == false) {

    if (yValue < maxThreshold && yValue > minThreshold) {
      currentMove = moves[RIGHT];

    }

    else if (yValue >= maxThreshold) {
      if (xValue > yValue) {
        //the joyStick is more on the right than it is upwards
        currentMove = moves[RIGHT];
      } else {
        currentMove = moves[UP];
      }
    }

    else if (yValue <= minThreshold) {
      //the joyStick is more on the right than it is downwards
      //the joyStick is closer to the threshold for the right than the threashold for the down
      if (maxThreshold - xValue < yValue - minThreshold) {
        currentMove = moves[RIGHT];
      } else {
        currentMove = moves[DOWN];
      }
    }

    joyMoved = true;
  }

  //Left direction
  if (xValue < minThreshold && joyMoved == false) {

    if (yValue < maxThreshold && yValue > minThreshold) {
      currentMove = moves[LEFT];
    } else if (yValue >= maxThreshold) {
      if (xValue - minThreshold < maxThreshold - yValue) {
        currentMove = moves[LEFT];
      } else {
        currentMove = moves[UP];
      }
    }

    else if (yValue <= minThreshold) {
      if (xValue < yValue) {
        currentMove = moves[LEFT];
      } else {
        currentMove = moves[DOWN];
      }
    }

    joyMoved = true;
  }

  //Down direction
  if (yValue < minThreshold && joyMoved == false) {

    if (xValue < maxThreshold && xValue > minThreshold) {
      currentMove = moves[DOWN];
    } else if (xValue >= maxThreshold) {
      if (maxThreshold - xValue < yValue - minThreshold) {
        currentMove = moves[RIGHT];
      } else {
        currentMove = moves[DOWN];
      }
    }

    else if (xValue <= minThreshold) {
      if (xValue < yValue) {
        currentMove = moves[LEFT];
      } else {
        currentMove = moves[DOWN];
      }
    }

    joyMoved = true;
  }

  //Up direction
  if (yValue > maxThreshold && joyMoved == false) {

    if (xValue < maxThreshold && xValue > minThreshold) {
      currentMove = moves[UP];
    } else if (xValue >= maxThreshold) {
      if (xValue > yValue) {
        currentMove = moves[RIGHT];
      } else {
        currentMove = moves[UP];
      }
    }

    else if (xValue <= minThreshold) {
      if (xValue - minThreshold < maxThreshold - yValue) {
        currentMove = moves[LEFT];
      } else {
        currentMove = moves[UP];
      }
    }

    joyMoved = true;
  }

  if (xValue >= idleMin && xValue <= idleMax && yValue >= idleMin && yValue <= idleMax) {
    joyMoved = false;
  }
}
