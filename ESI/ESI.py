import gi
gi.require_version('Gtk', '3.0')
from gi.repository import Gtk, Gdk

#for ps
import subprocess
from subprocess import Popen, PIPE

class App:
	def __init__(self):
		self.builder = Gtk.Builder()
		self.builder.add_from_file("Gui.glade")
		self.builder.connect_signals(SignalHandler())
		
class FileChooserWindow(Gtk.Window):

    def __init__(self):
        Gtk.Window.__init__(self, title="FileChooser")

        box = Gtk.Box(spacing=6)
        self.add(box)

        button1 = Gtk.Button("Choose File")
        button1.connect("clicked", self.on_file_clicked)
        box.add(button1)

        button2 = Gtk.Button("Choose Folder")
        button2.connect("clicked", self.on_folder_clicked)
        box.add(button2)

    def on_file_clicked(self, widget):
        dialog = Gtk.FileChooserDialog("Please choose a file", self,
            Gtk.FileChooserAction.OPEN,
            (Gtk.STOCK_CANCEL, Gtk.ResponseType.CANCEL,
             Gtk.STOCK_OPEN, Gtk.ResponseType.OK))

        self.add_filters(dialog)

        response = dialog.run()
        if response == Gtk.ResponseType.OK:
            print("Open clicked")
            print("File selected: " + dialog.get_filename())
        elif response == Gtk.ResponseType.CANCEL:
            print("Cancel clicked")

        dialog.destroy()

    def add_filters(self, dialog):
        filter_text = Gtk.FileFilter()
        filter_text.set_name("Text files")
        filter_text.add_mime_type("text/plain")
        dialog.add_filter(filter_text)

        filter_py = Gtk.FileFilter()
        filter_py.set_name("Python files")
        filter_py.add_mime_type("text/x-python")
        dialog.add_filter(filter_py)

        filter_any = Gtk.FileFilter()
        filter_any.set_name("Any files")
        filter_any.add_pattern("*")
        dialog.add_filter(filter_any)

    def on_folder_clicked(self, widget):
        dialog = Gtk.FileChooserDialog("Please choose a folder", self,
            Gtk.FileChooserAction.SELECT_FOLDER,
            (Gtk.STOCK_CANCEL, Gtk.ResponseType.CANCEL,
             "Select", Gtk.ResponseType.OK))
        dialog.set_default_size(800, 400)

        response = dialog.run()
        if response == Gtk.ResponseType.OK:
            print("Select clicked")
            print("Folder selected: " + dialog.get_filename())
        elif response == Gtk.ResponseType.CANCEL:
            print("Cancel clicked")

        dialog.destroy()	
		
class SignalHandler:
	def __init__(self):
		self.option_A = False
		self.option_F = False
		self.option_U = False
		self.option_L = False
		self.option_X = False
		self.option_E = False
		self.firstTime = True

	def onDestroy(self, *args):
		Gtk.main_quit()

	def on_Quit_activate(self, *args):
		Gtk.main_quit()
		
	def on_MenuSave_activate(self, *args):
		x = FileChooserWindow()
		
	def on_MenuOpen_activate(self, *args):
		x = FileChooserWindow()
	
	def on_ActionBo_changed(self, ActionBox):
			
		if ActionBox.get_active() == 1:
			app.builder.get_object("OutputText").get_buffer().set_text("")
			
		elif ActionBox.get_active() == 2:
			start = app.builder.get_object("OutputText").get_buffer().get_start_iter()
			end = app.builder.get_object("OutputText").get_buffer().get_end_iter()
			clipboard = Gtk.Clipboard.get(Gdk.SELECTION_CLIPBOARD)
			clipboard.set_text(app.builder.get_object("OutputText").get_buffer().get_text(start,end,True), len = -1)
			clipboard.store()			
			
		else:
			pass

	def on_ExecuteButton_clicked(self, ExecuteButton):
		psArgs = app.builder.get_object("PSArgs").get_text()
		print(psArgs)
		if not psArgs:

			if self.option_A:
				psArgs += "a"

			if self.option_F:
				psArgs += "f"

			if self.option_U:
				psArgs += "u"
		
			if self.option_L:
				psArgs += "l"

			if self.option_X:
				psArgs += "x"

			if self.option_E:
				psArgs += "e"
					
		
		if self.firstTime == True:
			app.builder.get_object("ActionBox").set_opacity(1.0)
			app.builder.get_object("OutputText").set_opacity(1.0)
			self.firstTime = False
		
		
		if not psArgs:
			proc = subprocess.Popen(['ps'], stdout = subprocess.PIPE)
			
		else:
			proc = subprocess.Popen(['ps', psArgs], stdout = subprocess.PIPE)

		out, err = proc.communicate()
		app.builder.get_object("OutputText").get_buffer().set_text(out.decode("utf-8"))
		app.builder.get_object("OutputText").get_buffer().get_end_iter()
			

	def on_Basic_clicked(self, Basic):
		app.builder.get_object("OptionsCheckBoxes").set_opacity(1.0)
		app.builder.get_object("PSArgs").set_opacity(0.0)
	
	def on_CommandLine_clicked(self, CommandLine):
		app.builder.get_object("PSArgs").set_opacity(1.0)
		app.builder.get_object("OptionsCheckBoxes").set_opacity(0.0)

	def on_Options_A_clicked(self, Options_A):
		if Options_A.get_active():
			self.option_A = True

		else:
			self.option_A = False

	def on_Options_F_clicked(self, Options_F):
		if Options_F.get_active():
			self.option_F = True

		else:
			self.option_F = False

	def on_Options_U_clicked(self, Options_U):
		if Options_U.get_active():
			self.option_U = True

		else:
			self.option_U = False

	def on_Options_L_clicked(self, Options_L):
		if Options_L.get_active():
			self.option_L = True
		
		else:
			self.option_L = False

	def on_Options_X_clicked(self, Options_X):
		if Options_X.get_active():
			self.option_X = True

		else:
			self.option_X = False

	def on_Options_E_clicked(self, Options_E):
		if Options_E.get_active():
			self.option_E = True

		else:
			self.option_E = False
			
if __name__ == "__main__":
	app = App()
	window = app.builder.get_object("MainWindow")
	window.show_all()
	Gtk.main()
