package msk_project;

import java.util.concurrent.TimeUnit;

public class Processor
{
    private int process_id;
        public enum State
        {
                Idle, Working, Finished
        };

        private int id = -1;
        private State m_state = State.Idle;

        public Processor(int di)
        {
                id = di;
        }

        public void setState(State s)
        {
                m_state = s;
        }

        public int getState()
        {
                return m_state.ordinal();
        }

        public int getID()
        {
                return id;
        }

        public int getProcessID()
        {
            return process_id;
        }

        public void startWork(int ps_id)
        {
            process_id = ps_id;
            m_state = State.Working;

               Thread th = new Thread(){
                    public void run()
                    {
                        try
                        {
                        TimeUnit.SECONDS.sleep(1);
                        }
                    catch(InterruptedException e)
                    {
                         // this part is executed when an exception (in this example InterruptedException) occurs
                    }

                m_state = Processor.State.Finished;
                    }

                        };
            th.start();
        }
}
