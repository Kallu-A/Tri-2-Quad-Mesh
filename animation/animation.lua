-- Lua (Keep this comment, this is an indication for editor's 'run' command)
  path = '/home/lucas/Documents/Tri-2-Quad-Mesh/animation/'
  for i=0,scene_graph.nb_children-1 do
    scene_graph.ith_child(i).visible = false
  end

  for i=0,scene_graph.nb_children-1 do
    obj = scene_graph.ith_child(i)
    obj.visible = true

  obj.shader.painting  = 'ATTRIBUTE'
  -- obj.shader.mesh_style='true; 0 0 0 1; 1'
  --  obj.query_interface('OGF::MeshGrobAttributesCommands').compute_vertices_id()
  obj.shader.attribute = 'facets.group_number'
  if (i == scene_graph.nb_children-1) then
    obj.shader.attribute = 'vertices.border_group'
  end
  obj.shader.colormap = 'rainbow'
  obj.shader.autorange()



    main.draw()
    main.render_area.update()
    name = string.format("%05.1f",i)
    print(name)
    main.render_area.snapshot(path..name..'.png')
    obj.visible = false
  end
  --os.execute("convert -resize 40% -set delay 1x100 -loop 0 /home/ely/gitlab/ultimailleBunin/animation/*.png /home/ely/gitlab/ultimailleBunin/animation/1output.gif")
 os.execute("convert -delay 15 -loop 1 $(\\ls "..path.."*.png | head -n) "..path.."output.gif")

