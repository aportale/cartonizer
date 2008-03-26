<map version="0.9.0_Beta_8">
<!-- To view this file, download free mind mapping software FreeMind from http://freemind.sourceforge.net -->
<node CREATED="1204508256515" ID="ID_1118980450" MODIFIED="1204508269906" TEXT="Cartonizer">
<node CREATED="1204508280484" ID="ID_40406398" MODIFIED="1204509338953" POSITION="right" TEXT="Optimizations">
<node CREATED="1204508391109" ID="ID_1709286744" MODIFIED="1204508937406" TEXT="Caching reflections&#xa;">
<node CREATED="1204508937421" ID="ID_861284437" MODIFIED="1204508937421" TEXT="It seems that composing the temporary reflection pixmap takes a considerable amount of time. Caching a reflection pixmap for every side could bring speedup. Each time a face for a side changes, that cache pixmap needs to be refreshed."/>
</node>
<node CREATED="1204508912234" ID="ID_446477468" MODIFIED="1204508965031" TEXT="Speed up animated preview">
<node CREATED="1204508970968" ID="ID_1881149404" MODIFIED="1204509750421" TEXT="All quality features (antialiasing, etc.) could be deactiveated in general as long as a property changement comes from the application controller. The view could when it is idle request a repaint with full quality. Qts documentation suggests to use a QTimer with 0 timeout.">
<arrowlink DESTINATION="ID_241507512" ENDARROW="Default" ENDINCLINATION="168;0;" ID="Arrow_ID_979025935" STARTARROW="None" STARTINCLINATION="168;0;"/>
</node>
</node>
</node>
<node CREATED="1204509361843" ID="ID_1349888517" MODIFIED="1204509369015" POSITION="right" TEXT="Rendering quality">
<node CREATED="1204509380937" ID="ID_241507512" MODIFIED="1204509750421" TEXT="Qt 4.4 fixes the QTransform&apos;ed bitmap rendering. However, a heavily X- or Y- axis rotated pixmap renders in in a non-smooth way, even with smoothpixmaptransform. In order to optain a high quality rendering, an offscreen image wit ha n-times higer resolution could be painted (without antialiasing and smoothpixmaptransform) and then resampled to an n-th of the resolution."/>
</node>
<node CREATED="1204509767453" ID="ID_8191394" MODIFIED="1204509771500" POSITION="left" TEXT="Features">
<node CREATED="1204509775484" ID="ID_715859945" MODIFIED="1204509817781" TEXT="Multi face loading">
<node CREATED="1204509844296" ID="ID_1897096243" MODIFIED="1204510736171" TEXT="Multiple faces can come from one image. That makes designing elements across faces (e.g. an images that goes across front and top face) in graphical editors easier. It also reduces the amount of files to handle to one. It requires however, an extra step where the locations of the single faces inside that image are defined. If the faces are adjacent in a predefined layout, the user could have a GUI where he simply defines the border lines between left-front, top-front and front-right. If the faces could be everywhere in the input image, the user needs to define the 4 rectangles. An algorithm could also try to determine the borders of the adjacent faces by measuring the free upper left square conjoined with the left ant top face."/>
</node>
<node CREATED="1204510431609" ID="ID_970811459" MODIFIED="1204510462578" TEXT="Faces template file wizard">
<node CREATED="1204510466781" ID="ID_377671574" MODIFIED="1204510818140" TEXT="A wizard could help to generate an empty image file with the required dimensions. The unused squares in the upper left and upper right could be gray and contain labels with arrows for the conjoined faces. Those gray squares would also indicate the borders between the faces. Also svg could be generated by the wizard, and it could contain tree guidelines in sodipodi format.">
<arrowlink DESTINATION="ID_1897096243" ENDARROW="Default" ENDINCLINATION="105;0;" ID="Arrow_ID_1396006045" STARTARROW="None" STARTINCLINATION="105;0;"/>
</node>
</node>
<node CREATED="1204511265312" ID="ID_1505906357" MODIFIED="1204511279906" TEXT="Visual box rotation">
<node CREATED="1204511282500" ID="ID_1095569836" MODIFIED="1204511537375" TEXT="The view should allow to modify the &quot;camera position&quot; via drag an drop, by virtually rotating the box. Two modes (selectable with toolbuttons): 1) Rotation, horizontal dragging rotates, vertical dragging inclines 2) Height, vertical dragging modifies the height of the &quot;camera position&quot;."/>
</node>
</node>
</node>
</map>