module Shaders = Revery_Shaders;
module Geometry = Revery_Geometry;

module Layout = Layout;
module LayoutTypes = Layout.LayoutTypes;
module Style = Style;
module Components = Components;

class viewNode = ViewNode.viewNode;
class textNode = TextNode.textNode;
class imageNode = ImageNode.imageNode;


module UiReact = Reactify.Make(UiReconciler);

open UiReconciler;
let view = (~children, ~style=Style.defaultStyle, ()) => 
    UiReact.primitiveComponent(View(style), ~children);

type uiContainer = {
    rootNode: viewNode,
    container: UiReact.container,
};

let create = () => {
     let rootNode = new viewNode ("root");
     let container = UiReact.createContainer(rootNode);
     let ret: uiContainer = { rootNode, container };
    ret;
};

let layout = (node) => {
    let rootLayoutNode = node#toLayoutNode();
    Layout.layoutNode(rootLayoutNode);
};

let render = (container: uiContainer, component: UiReact.component) => {
    let { rootNode, container } = container;
    UiReact.updateContainer(container, component);

    layout(rootNode);
    rootNode#draw(0);
};
