# ofxMeshWarp
Warping mesh points with mouse and keyboard.  
You can do it by code, of course.

## How to use
### code
See example

### selecting points
You can select/deselect point(s) by clicking on the point or making rectangle including the points.  
With shift key pressed, additive. (???)
With command key pressed, alternative. (???)

### moving points
You can move points' position by dragging on a selected point.  
With ALT key pressed, texcoord will be moved instead.  
With shift key pressed, points move according axis.  
You can also move by arrow keys.

### editing alpha
You can edit points' alpha by scrolling Y-axis. (???)

### switching nodal or not
Each point has a property of nodal or not.  
By clicking on a point with ALT key pressed, you can switch it.  
If a point is nodal, you can control its position or texcoord directly.  
If a point is not nodal, you cannot move it but will be controlled by the adjacent points.

### dividing or reducing mesh points
Click on a line with CONTROL key pressed to divide mesh.  
With ALT key pressed, reduce.

_**warning**  
Dividing mesh allocates memory for new points but reducing doesn't free them.  
So if you call divide/reduce many times, you may need to call `ofxMeshWarp::gc()` on reasonable time._


### Save/Load
`s` and `l` keys.

## License
MIT license.