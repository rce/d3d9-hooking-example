# d3d9-hooking-example

`d3d9-example-target` is an example application that implements basic render
loop that calls the following functions:

- `IDirect3DDevice9::Clear(...)`
- `IDirect3DDevice9::BeginScene()`
- `IDirect3DDevice9::EndScene()`
- `IDirect3DDevice9::Present(...)`

Usually the reason to hook Direct3D is to render something on every frame. In
practice this means you'll want to render whatever you want before `Present`
is called. Usually hooking either `EndScene` or `Present` works well for this
purpose.

## Reference

- http://www.directxtutorial.com/LessonList.aspx?listid=9