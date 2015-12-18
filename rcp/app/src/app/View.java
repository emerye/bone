package app;

import java.util.Calendar;
import java.util.Date;
import java.util.Timer;
import java.util.TimerTask;
import java.text.DateFormat; 

import org.eclipse.jface.viewers.IStructuredContentProvider;
import org.eclipse.jface.viewers.ITableLabelProvider;
import org.eclipse.jface.viewers.LabelProvider;
import org.eclipse.jface.viewers.TableViewer;
import org.eclipse.jface.viewers.Viewer;
import org.eclipse.swt.SWT;
import org.eclipse.swt.graphics.Font;
import org.eclipse.swt.graphics.FontData;
import org.eclipse.swt.graphics.Image;
import org.eclipse.swt.layout.GridLayout;
import org.eclipse.swt.layout.RowLayout;
import org.eclipse.swt.widgets.Composite;
import org.eclipse.swt.widgets.Display;
import org.eclipse.swt.widgets.Group;
import org.eclipse.swt.widgets.Label;
import org.eclipse.ui.ISharedImages;
import org.eclipse.ui.PlatformUI;
import org.eclipse.ui.part.ViewPart;
import org.eclipse.ui.forms.widgets.FormToolkit; 

public class View extends ViewPart {
	public static final String ID = "app.view";

	private Label lblDate; 
	private Timer timer; 
	private FormToolkit toolKit; 

	/**
	 * This is a callback that will allow us to create the viewer and initialize
	 * it.
	 */
	public void createPartControl(Composite parent) {
		
		Date curDate = new Date();
		Calendar curTime = Calendar.getInstance();
		
		FontData defaultFont = new FontData("Courier",36,SWT.BOLD);
		Font font = new Font(parent.getDisplay(), defaultFont);
		String version = Display.getAppVersion();
		toolKit = new FormToolkit(Display.getDefault());
		
		RowLayout rowLayout = new RowLayout(); 
		rowLayout.wrap = true;
		rowLayout.pack = false; 
		parent.setLayout(rowLayout);
//		toolKit.adapt(parent);
		
		lblDate = new Label(parent, SWT.NONE); 
		lblDate.setFont(font);
	//	lblDate.setText(curDate.toString());
		lblDate.setText(DateFormat.getInstance().format(curTime.getTime()));
		
		timer = new Timer(); 
//		timer.schedule(new ClockUpdate(), 0, 1000); 
		

		Group grpClock = new Group(parent, SWT.NONE);
	
		grpClock.setLayout(new GridLayout(2, false));
		grpClock.setText("Group Box");
		
		Label lblGroup = new Label(grpClock,SWT.NONE);
		lblGroup.setText("App Version" + version);
		
		Label lblGroup1 = new Label(grpClock,SWT.NONE);
		lblGroup1.setText("A label");
		
		Label lblGroup2 = new Label(grpClock,SWT.NONE);
		lblGroup2.setText("Label in Group 2");
	
		/*
		Composite composite = new Composite(grpClock, SWT.NONE); 
		Label lblComp = new Label(composite, SWT.NONE);
		composite.setSize(500,500);
		lblComp.setText("Label in Composite.");
		*/
		
		
	}

//This does not make any difference. 
	public void finalize() {
		timer.cancel(); 
		System.out.println("Destroy called.");
	}
	

	/**
	 * Passing the focus request to the viewer's control.
	 */
	public void setFocus() {	
	}

	public Label getLblDate() {
		return lblDate;
	}

	public void setLblDate(Label lblDate) {
		this.lblDate = lblDate;
	}
	
	class ClockUpdate extends TimerTask {
		int count = 0; 
		Date now = new Date(); 
		
		public void run() {
			final Date now = new Date();
	//		System.out.println(Integer.toString(count));
			Display.getDefault().syncExec(new Runnable() {
				  public void run() {
				    lblDate.setText(now.toString());
				  }
				});
			count += 1; 
		}
	}
}