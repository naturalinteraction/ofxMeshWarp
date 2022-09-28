# ofxMeshWarp
Warping mesh points with mouse and keyboard.  
You can do it by code, of course.

# poll machine
Password for teamviewer, bios and sudo is:
```
youmustfollowthewhiterabbit
```

### elevation warping
`w,s` scale up/down
`q,z` center of projection up/down
`a,d` elevation multiplier up/down

### selecting points
You can select/deselect point(s) by clicking on the point or making rectangle including the points.  

### moving points
You can move points' position by dragging on a selected point.  
With ALT key pressed, texcoord will be moved instead.  

### editing alpha
You can edit selected points' alpha by mouse scrolling.

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
`S` and `L` keys.

## License
MIT license.

### Git Credentials
Stored by default in `~/.git-credentials` in plain text.