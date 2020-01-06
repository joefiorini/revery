open Revery;
open ExampleStubs;

module SliderExample = Slider;
module ScrollViewExample = ScrollView;

open Revery.UI;
open Revery.UI.Components;

let bgColor = Color.hex("#212733");
let activeBackgroundColor = Color.hex("#2E3440");
let inactiveBackgroundColor = Color.hex("#272d39");
let selectionHighlight = Color.hex("#90f7ff");

type example = {
  name: string,
  render: Window.t => React.element(React.node),
  source: string,
};

type state = {
  examples: list(example),
  selectedExample: string,
};

let state: state = {
  examples: [
    {name: "Animation", render: _w => Hello.render(), source: "Hello.re"},
    {
      name: "Spring",
      render: _w => SpringExample.render(),
      source: "SpringExample.re",
    },
    {
      name: "CanQuit",
      render: _ => CanQuitExample.render(),
      source: "CanQuit.re",
    },
    {
      name: "Button",
      render: _ => DefaultButton.render(),
      source: "DefaultButton.re",
    },
    {
      name: "Checkbox",
      render: _ => CheckboxExample.render(),
      source: "CheckboxExample.re",
    },
    {
      name: "Slider",
      render: _ => SliderExample.render(),
      source: "Slider.re",
    },
    {name: "Border", render: _ => Border.render(), source: "Border.re"},
    {
      name: "ScrollView",
      render: _ => ScrollViewExample.render(),
      source: "ScrollView.re",
    },
    {
      name: "Calculator",
      render: w => Calculator.render(w),
      source: "Calculator.re",
    },
    {name: "Flexbox", render: _ => Flexbox.render(), source: "Flexbox.re"},
    {
      name: "Box Shadow",
      render: _ => Boxshadow.render(),
      source: "Boxshadow.re",
    },
    {name: "Focus", render: _ => FocusExample.render(), source: "Focus.re"},
    {name: "Fonts", render: _ => FontsExample.render(), source: "Fonts.re"},
    {
      name: "Stopwatch",
      render: _ => Stopwatch.render(),
      source: "Stopwatch.re",
    },
    {
      name: "Native: File(s)/Folders(s)",
      render: _ => NativeFileExample.render(),
      source: "NativeFileExample.re",
    },
    {
      name: "Native: Notifications",
      render: _ => NativeNotificationExample.render(),
      source: "NativeNotificationExample.re",
    },
    {
      name: "Input",
      render: _ => InputExample.render(),
      source: "InputExample.re",
    },
    {
      name: "Radio Button",
      render: _ => RadioButtonExample.render(),
      source: "RadioButtonExample.re",
    },
    {
      name: "Game Of Life",
      render: _ => GameOfLife.render(),
      source: "GameOfLife.re",
    },
    {
      name: "Screen Capture",
      render: w => ScreenCapture.render(w),
      source: "ScreenCapture.re",
    },
    {
      name: "Tree View",
      render: w => TreeView.render(w),
      source: "TreeView.re",
    },
    {
      name: "Analog Clock",
      render: _ => AnalogClock.render(),
      source: "AnalogClock.re",
    },
    {
      name: "TodoMVC",
      render: _ => TodoExample.render(),
      source: "TodoExample.re",
    },
    {
      name: "Dropdown",
      render: _ => DropdownExample.render(),
      source: "DropdownExample.re",
    },
    {
      name: "Text",
      render: _w => TextExample.render(),
      source: "TextExample.re",
    },
    {
      name: "Hover Example",
      render: _ => HoverExample.render(),
      source: "HoverExample.re",
    },
    {
      name: "OpenGL Example",
      render: _ => OpenGLExample.render(),
      source: "OpenGLExample.re",
    },
    {
      name: "Zoom Example",
      render: _ => ZoomExample.render(),
      source: "ZoomExample.re",
    },
  ],
  selectedExample: "Button",
};

let initState = state;

let getExampleByName = (state: state, example: string) =>
  List.filter(x => String.equal(x.name, example), state.examples) |> List.hd;

let getSourceForSample = (state: state, example: string) =>
  getExampleByName(state, example) |> (s => s.source);

let noop = () => ();

let getRenderFunctionSelector: (state, string, Window.t) => React.element(React.node) =
  (s: state, selectedExample) => getExampleByName(s, selectedExample) |> (a => a.render);

module ExampleButton = {
  let make = (~isActive, ~name, ~onClick, ()) => {
    let highlightColor =
      isActive ? selectionHighlight : Colors.transparentWhite;

    let buttonOpacity = 1.0;
    let bgColor = isActive ? activeBackgroundColor : inactiveBackgroundColor;

    let wrapperStyle =
      Style.[
        borderLeft(~width=4, ~color=highlightColor),
        backgroundColor(bgColor),
      ];

    let textColor = isActive ? Colors.white : Colors.grey;
    let textHeaderStyle =
      Style.[
        color(textColor),
        fontFamily("Roboto-Regular.ttf"),
        fontSize(14),
        margin(16),
      ];

    <Opacity opacity=buttonOpacity>
      <Clickable style=wrapperStyle onClick>
        <Text style=textHeaderStyle text=name />
      </Clickable>
    </Opacity>;
  };
};

type action =
  | SelectExample(string);

let reducer = (action: action, state: state) =>
  switch (action) {
  | SelectExample(name) => {...state, selectedExample: name}
  };

Revery_Core.Event.dispatch(Revery.UI.hotReload, ());

module ExampleHost = {
  let%component make = (~win, ~setGen, ()) => {
    let%hook (state, dispatch) = Hooks.reducer(~initialState=state, reducer);
    let%hook () = Hooks.effect(If((!=), Hook_p.gen^), () => {
        Printf.printf("Hook_p.gen^ <> Hook_p.gen^\n%!");
        setGen(gen => gen + 1);
        None
    });

    let renderButton = (x: example) => {
      let isActive = String.equal(x.name, state.selectedExample);
      <ExampleButton
        isActive
        name={x.name}
        onClick={_ => {
          Window.setTitle(win, "Revery Example - " ++ x.name);

          let sourceFile = getSourceForSample(state, x.name);
          prerr_endline("SOURCE FILE: " ++ sourceFile);
          notifyExampleSwitched(sourceFile);
          dispatch(SelectExample(x.name));
          ();
        }}
      />;
    };

    let buttons = List.map(renderButton, state.examples);

    let exampleRender = getRenderFunctionSelector(initState, state.selectedExample);
    let example = exampleRender(win);

    <View
      onMouseWheel={_evt => ()}
      style=Style.[
        position(`Absolute),
        justifyContent(`Center),
        alignItems(`Center),
        backgroundColor(bgColor),
        bottom(0),
        top(0),
        left(0),
        right(0),
        flexDirection(`Row),
      ]>
      <ScrollView
        style=Style.[
          position(`Absolute),
          top(0),
          left(0),
          width(175),
          bottom(0),
          backgroundColor(bgColor),
        ]>
        <View> {buttons |> React.listToElement} </View>
      </ScrollView>
      <View
        style=Style.[
          position(`Absolute),
          top(0),
          left(175),
          right(0),
          bottom(0),
          backgroundColor(activeBackgroundColor),
        ]>
        example
      </View>
    </View>;
  };
};

Hook_p.view := ExampleHost.make;

Hook_p.increment := (s => s + 1);

incr(Hook_p.gen);