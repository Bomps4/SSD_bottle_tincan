# ON DRONED DEPLOYMENT 

This section shows the commands necessary for the deployment of a quantized neural network on Gap8.

## TO DEPLOY  ON DRONE
 
You need to source the correct configuration this can be done with the 

`source path/to/gap_sdk/configs/ai_deck.sh`

once this is done you need to export the GAPY_OPENOCD_CABLE which can be done with the command

`export GAPY_OPENOCD_CABLE=path/to/gap_sdk/utils/gap8-openocd/tcl/interface/ftdi/olimex-arm-usb-ocd-h.cfg`

the last step for the deployment is to use the :
`make clean all run`
and run the application on the drone.

## VISUALIZATION

To  visualize the detections being performed by the drone you need first to connect to the Bitcraze AI-deck example wi-fi network.The script requires the gi package which can be installed following this [GUIDE](https://pygobject.readthedocs.io/en/latest/getting_started.html#ubuntu-getting-started)
 Then you need to run the viewer_custom.py with :
```python viewer_custom ```


 


	 





