import QtQuick 2.12

Rectangle {
   id: root
   property alias internalText: valueText // <- or we can bind text from the model like that, but it is not preferable
   color: "lightgreen"
   border.color: "black"
   border.width: 1
   radius: 5

   Text {
       id: valueText //use _ as a first id name symbol

       anchors.centerIn: root
       text: "1"
       font{
            pointSize: root.height * 0.33
            bold: true
       }

   }
}
