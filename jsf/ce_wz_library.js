

//Integrated to Walter Zorn html library

// function executeTest: for testing purposes on layerObject.htm
function executeTest(){

transformLayer (document.forms[0].x.value,document.forms[0].y.value,document.forms[0].w.value,document.forms[0].h.value,"Bloque",100,10);


}

// Uses WZ dhtml library to make timed transformations of dhtml,
// so minimizing, maximizing layers can be rather "smoot" for user,
// simulating windows os.

//Function transformLayer
//x,y = Position. Top left corner coordinates
//w,h = Size. width and height of <div></div> layer
//id = id of the Div (i.e.: id="myLayer", then, use "myLayer", WITH quotes!)
//duration = Duration, in miliseconds
//steps = Transition intervals

function transformLayer (x,y,w,h,id,duration,steps) {
  
  //duration is in seconds
  stepDuration = Math.round(duration/steps) ; // Value is in miliseconds.

  obj = dd.elements[id];
  

  // Get original values: x,y = top left corner;  w,h = width height   
  
  x1 = obj.x;  
  y1 = obj.y;  
  w1 = obj.w;  
  h1 = obj.h;  


  // If values not set, or zero, we do not modify them, and take original as final as well
  x = (x)?x:x1;
  y = (y)?y:y1;
  w = (w)?w:w1;
  h = (h)?h:h1;

  // how much do we need to modify our values for each step?
  // The WZ library moveBy and resizeBy takes the diff with inverted sign as from original ce script:
  difX = (x - x1)/steps;  
  difY = (y - y1)/steps;  
  difW = (w - w1)/steps;  
  difH = (h - h1)/steps;  
  
  
  
  if(top.debug==true)  window.alert('x1:'+x1+', y1:'+y1+', w1:'+w1+', h1:'+h1+',<br>x:'+x+', y:'+y+', w:'+w+', h:'+h+', <br>difX:'+difX+', difY:'+difY+', difW:'+difW+', difH:'+difH+',<br>duration:'+duration+', <br>steps:'+steps+',<br>stepDuration:'+stepDuration+'');
 
  obj.moveBy(difX,difY);
  obj.resizeBy(difW,difH);

  //We take out the amount of time used, and a step already being executed
  duration = (duration - stepDuration);
  steps --;

	//If there is any step left, we execute again
	if(steps>=0){
	 setTimeout('transformLayer('+x+','+y+','+w+','+h+',"'+id+'",'+duration+','+steps+')',stepDuration);
	 }
  //If Return... we could set some api here as to call a function to be evaluated by the javascript,if set, on return
  //for example to call the function that has to populate the div with content
  //but WZ has already thought on it :-)
  return;

}

